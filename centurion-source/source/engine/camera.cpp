#include <engine>
#include <game>
#include <interface>

using namespace glb;
using namespace engine;
using namespace game;
using namespace editor;

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch){
	position = startPosition;

	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	update();
}

void Camera::update(){
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::mouseControl(float threshold) {

	abs_x = getParam("mouse-x-position") + position.x;
	abs_y = getParam("mouse-y-position") + position.y;

	threshold_x = mapWidth - 2 * settings.GetCameraMovespeed() + (getParam("window-width-zoomed") - settings.GetWindowWidth());
	threshold_y = mapHeight - 2 * settings.GetCameraMovespeed() + (getParam("window-height-zoomed") - settings.GetWindowHeight());

	float threshold_top = threshold;
	if (ENGINE()->getEnvironment() == "editor") threshold_top += 30.f;

	//Left margin
	if (getParam("mouse-x-position") <= threshold && (abs_x > threshold) && getParam("mouse-x-position") > 0) {
		position -= right * settings.GetCameraMovespeed();
	}
	//Right margin
	if (getParam("mouse-x-position") >= (settings.GetWindowWidth() - threshold) && (abs_x < threshold_x) && getParam("mouse-x-position") < settings.GetWindowWidth()) {
		if (position.x < mapWidth - getParam("window-width-zoomed")) {
			position += right * settings.GetCameraMovespeed();
		}
	}
	//Top margin
	if (getParam("mouse-y-position") >= (settings.GetWindowHeight() - threshold_top) && abs_y < (threshold_y + getParam("ui-top-height")* getParam("window-height-zoomed") / getParam("window-height-zoomed")) && getParam("mouse-y-position") < settings.GetWindowHeight()) {
		if (position.y < (mapHeight - getParam("window-height-zoomed") + getParam("ui-top-height")* getParam("window-height-zoomed") / settings.GetWindowHeight())) {
			position += up * settings.GetCameraMovespeed();
		}
	}
	//Bottom margin
	if (getParam("mouse-y-position") <= threshold && abs_y > (threshold - getParam("ui-bottom-height")* getParam("window-height-zoomed") / settings.GetWindowHeight()) && getParam("mouse-y-position") > 0) {
		if (position.y > (0 - getParam("ui-bottom-height")* getParam("window-height-zoomed") / settings.GetWindowHeight())) {
			position -= up * settings.GetCameraMovespeed();
		}
	}

	/* MOUSE SCROLLING --> CAMERA ZOOM */
	if (!gameMenuStatus && (KeyCode[GLFW_KEY_LEFT_CONTROL] || KeyCode[GLFW_KEY_RIGHT_CONTROL]) && getBoolean("mouse-scroll-bool")) {
		if (getParam("mouse-scroll") > 0 && currentZoomCamera > 1.0f) {
			currentZoomCamera -= (int)getParam("mouse-scroll");
		}

		else if (getParam("mouse-scroll") < 0 && currentZoomCamera < settings.GetCameraMaxZoom()) {
			currentZoomCamera -= (int)getParam("mouse-scroll");
		}
		setBoolean("mouse-scroll-bool", false);
	}

	if (position.x > mapWidth - getParam("window-width-zoomed") - settings.GetCameraMovespeed()) {
		position.x = mapWidth - getParam("window-width-zoomed") - settings.GetCameraMovespeed();
	}
	if (position.y < -getParam("ui-bottom-height") * getParam("window-height-zoomed") / settings.GetWindowHeight()) {
		position.y = -getParam("ui-bottom-height") * getParam("window-height-zoomed") / settings.GetWindowHeight();
	}
	if (position.y > mapHeight - getParam("window-height-zoomed") + getParam("ui-top-height") * getParam("window-height-zoomed") / settings.GetWindowHeight()) {
		position.y = mapHeight - getParam("window-height-zoomed") + getParam("ui-top-height") * getParam("window-height-zoomed") / settings.GetWindowHeight();
	}
	/* ------------------------------------------- */	
	setParam("window-width-zoomed", settings.GetWindowWidth() + (currentZoomCamera - 1) * zoomCameraFactor);
	setParam("window-height-zoomed", settings.GetWindowHeight() + (currentZoomCamera - 1) *  zoomCameraFactor / getParam("window-ratio"));
	setParam("camera-x-position", position.x);
	setParam("camera-y-position", position.y);
}

void Camera::keyboardControl() {
	//Left margin
	if (KeyCode[GLFW_KEY_LEFT] && position.x > 0) {
		position -= right * settings.GetCameraMovespeed();
	}
	//Right margin 
	if (KeyCode[GLFW_KEY_RIGHT] && position.x < mapWidth - getParam("window-width-zoomed")) {
		position += right * settings.GetCameraMovespeed();
	}
	//Top margin 
	if (KeyCode[GLFW_KEY_UP] && (position.y < (mapHeight - getParam("window-height-zoomed") + getParam("ui-top-height")* getParam("window-height-zoomed") / settings.GetWindowHeight()))) {
		position += up * settings.GetCameraMovespeed();
	}
	//Bottom margin 
	if (KeyCode[GLFW_KEY_DOWN] && (position.y > (0 - getParam("ui-bottom-height")* getParam("window-height-zoomed") / settings.GetWindowHeight()))) {
		position -= up * settings.GetCameraMovespeed();
	}

	setParam("camera-x-position", position.x);
	setParam("camera-y-position", position.y);
}

void Camera::go_to_pos(GLfloat x, GLfloat y) {
	if (x < 0.0) { x = 0; }
	else if (x > mapWidth - getParam("window-width-zoomed")) { x = mapWidth - getParam("window-width-zoomed"); }
	if (y < 0.0) { y = 0; }
	else if (y > mapHeight - getParam("window-height-zoomed")) { y = mapHeight - getParam("window-height-zoomed"); }
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
