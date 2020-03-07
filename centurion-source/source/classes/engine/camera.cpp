#include "camera.h"
#include "engine.h"
#include "mouse.h"
#include "window.h"
#include <game/strategy.h>
#include <interface>

using namespace glb;


namespace engine {

	// define static variables
	float Camera::MaxZoom = 20.f;
	float Camera::MovementSpeed = 10.f;
	vec3 Camera::position, Camera::front, Camera::up, Camera::right, Camera::worldUp;
	GLfloat Camera::yaw, Camera::pitch, Camera::threshold_x, Camera::threshold_y, Camera::abs_x, Camera::abs_y;
	float Camera::zoomCameraFactor = 100.f;
	int Camera::currentZoom = 8;
	// ---------- end definitions

	Camera::Camera() {}

	void Camera::Init(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch) {
		position = startPosition;

		worldUp = startUp;
		yaw = startYaw;
		pitch = startPitch;
		front = glm::vec3(0.0f, 0.0f, 1.0f);

		update();
	}

	void Camera::update() {
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		front = glm::normalize(front);

		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

	void Camera::mouseControl() {

		abs_x = Mouse::GetXPosition() + position.x;
		abs_y = Mouse::GetYPosition() + position.y;

		threshold_x = MEDIUM_MAP_WIDTH - 2 * MovementSpeed + (myWindow::WidthZoomed - myWindow::Width);
		threshold_y = MEDIUM_MAP_HEIGHT - 2 * MovementSpeed + (myWindow::HeightZoomed - myWindow::Height);

		float threshold_top = CAMERA_THRESHOLD;
		if (Engine::getEnvironment() == "editor") threshold_top += 30.f;

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
		if (!game::GameMenu::IsActive() && (KeyCode[GLFW_KEY_LEFT_CONTROL] || KeyCode[GLFW_KEY_RIGHT_CONTROL]) && Mouse::ScrollBool) {
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

	void Camera::keyboardControl() {
		//Left margin
		if (KeyCode[GLFW_KEY_LEFT] && position.x > 0) {
			position -= right * MovementSpeed;
		}
		//Right margin 
		if (KeyCode[GLFW_KEY_RIGHT] && position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
			position += right * MovementSpeed;
		}
		//Top margin 
		if (KeyCode[GLFW_KEY_UP] && (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
			position += up * MovementSpeed;
		}
		//Bottom margin 
		if (KeyCode[GLFW_KEY_DOWN] && (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
			position -= up * MovementSpeed;
		}
	}

	void Camera::GoToPoint(GLfloat x, GLfloat y) {
		if (x < 0.0) { x = 0; }
		else if (x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) { x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed; }
		if (y < 0.0) { y = 0; }
		else if (y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed) { y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed; }
		position.x = x;
		position.y = y;
	}

	glm::mat4 Camera::calculateViewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}

	Camera::~Camera()
	{
	}

}