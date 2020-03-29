#include <engine.h>

#include <interface>
#include <game/game.h>

#include <GLFW/glfw3.h>

// define static variables
float Engine::Camera::MaxZoom = 20.f;
float Engine::Camera::MovementSpeed = 10.f;
vec3 Engine::Camera::position, Engine::Camera::front, Engine::Camera::up, Engine::Camera::right, Engine::Camera::worldUp;
GLfloat Engine::Camera::yaw, Engine::Camera::pitch, Engine::Camera::threshold_x, Engine::Camera::threshold_y, Engine::Camera::abs_x, Engine::Camera::abs_y;
float Engine::Camera::zoomCameraFactor = 100.f;
int Engine::Camera::currentZoom = 8;
// ---------- end definitions

Engine::Camera::Camera() {}

vec2 Engine::Camera::GetZoomedCoords(float xCoord, float yCoord)
{
	float x = xCoord * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
	float y = yCoord * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
	return vec2(x, y);
}

void Engine::Camera::Init(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch) {
	position = startPosition;

	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	update();
}

void Engine::Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Engine::Camera::mouseControl() {

	abs_x = Mouse::GetXPosition() + position.x;
	abs_y = Mouse::GetYPosition() + position.y;

	threshold_x = MEDIUM_MAP_WIDTH - 2 * MovementSpeed + (myWindow::WidthZoomed - myWindow::Width);
	threshold_y = MEDIUM_MAP_HEIGHT - 2 * MovementSpeed + (myWindow::HeightZoomed - myWindow::Height);

	float threshold_top = CAMERA_THRESHOLD;
	if (Engine::GetEnvironment() == EDITOR_ENV) threshold_top += 30.f;

	//Left margin
	if (Mouse::GetXPosition() <= CAMERA_THRESHOLD && (abs_x > CAMERA_THRESHOLD) && Mouse::GetXPosition() > 0) {
		position -= right * MovementSpeed;
	}
	//Right margin
	if (Mouse::GetXPosition() >= (myWindow::Width - CAMERA_THRESHOLD) && (abs_x < threshold_x) && Mouse::GetXPosition() < myWindow::Width) {
		if (position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
			position += right * MovementSpeed;
		}
	}
	//Top margin
	if (Mouse::GetYPosition() >= (myWindow::Height - threshold_top) && abs_y < (threshold_y + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::HeightZoomed) && Mouse::GetYPosition() < myWindow::Height) {
		if (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height)) {
			position += up * MovementSpeed;
		}
	}
	//Bottom margin
	if (Mouse::GetYPosition() <= CAMERA_THRESHOLD && abs_y > (CAMERA_THRESHOLD - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height) && Mouse::GetYPosition() > 0) {
		if (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height)) {
			position -= up * MovementSpeed;
		}
	}

	/* MOUSE SCROLLING --> CAMERA ZOOM */
	if (!game::GameMenu::IsActive() && (Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT_CONTROL) || Keyboard::IsKeyNotReleased(GLFW_KEY_RIGHT_CONTROL)) && Mouse::ScrollBool) {
		if (Mouse::ScrollValue > 0 && currentZoom > 1.0f) {
			currentZoom -= (int)Mouse::ScrollValue;
		}

		else if (Mouse::ScrollValue < 0 && currentZoom < MaxZoom) {
			currentZoom -= (int)Mouse::ScrollValue;
		}
		Mouse::ScrollBool = false;
	}

	if (position.x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed - MovementSpeed) {
		position.x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed - MovementSpeed;
	}
	if (position.y < -myWindow::BottomBarHeight * myWindow::HeightZoomed / myWindow::Height) {
		position.y = -myWindow::BottomBarHeight * myWindow::HeightZoomed / myWindow::Height;
	}
	if (position.y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight * myWindow::HeightZoomed / myWindow::Height) {
		position.y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight * myWindow::HeightZoomed / myWindow::Height;
	}
	/* ------------------------------------------- */
	myWindow::WidthZoomed = myWindow::Width + (currentZoom - 1) * zoomCameraFactor;
	myWindow::HeightZoomed = myWindow::Height + (currentZoom - 1) *  zoomCameraFactor / myWindow::Ratio;
}

void Engine::Camera::keyboardControl() {
	//Left margin
	if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT) && position.x > 0) {
		position -= right * MovementSpeed;
	}
	//Right margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_RIGHT) && position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
		position += right * MovementSpeed;
	}
	//Top margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_UP) && (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
		position += up * MovementSpeed;
	}
	//Bottom margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_DOWN) && (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
		position -= up * MovementSpeed;
	}
}

void Engine::Camera::GoToPoint(GLfloat x, GLfloat y) {
	if (x < 0.0) { x = 0; }
	else if (x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) { x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed; }
	if (y < 0.0) { y = 0; }
	else if (y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed) { y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed; }
	position.x = x;
	position.y = y;
}

glm::mat4 Engine::Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

float Engine::Camera::GetYMinimapCoordinate(float y)
{
	return myWindow::Height * (y - myWindow::BottomBarHeight) / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
}

Engine::Camera::~Camera()
{
}
