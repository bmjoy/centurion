#include <engine>

using namespace glb;

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

	threshold_x = GAME::MAP_WIDTH - 2 * GAME::CAMERA_MOVESPEED + (getParam("window-width-zoomed") - getParam("window-width"));
	threshold_y = GAME::MAP_HEIGHT - 2 * GAME::CAMERA_MOVESPEED + (getParam("window-height-zoomed") - getParam("window-height"));

	//Left margin
	if (getParam("mouse-x-position") <= threshold && (abs_x > threshold) && getParam("mouse-x-position") > 0) {
		position -= right * GAME::CAMERA_MOVESPEED;
	}
	//Right margin
	if (getParam("mouse-x-position") >= (getParam("window-width") - threshold) && (abs_x < threshold_x) && getParam("mouse-x-position") < getParam("window-width")) {
		if (position.x < GAME::MAP_WIDTH - getParam("window-width-zoomed")){
			position += right * GAME::CAMERA_MOVESPEED;
		}
	}
	//Top margin
	if (getParam("mouse-y-position") >= (getParam("window-height") - threshold) && abs_y < (threshold_y + getParam("ui-top-height")* getParam("window-height-zoomed") / getParam("window-height-zoomed")) && getParam("mouse-y-position") < getParam("window-height")) {
		if (position.y < (GAME::MAP_HEIGHT - getParam("window-height-zoomed") + getParam("ui-top-height")* getParam("window-height-zoomed") / getParam("window-height"))) {
			position += up * GAME::CAMERA_MOVESPEED;
		}
	}
	//Bottom margin
	if (getParam("mouse-y-position") <= threshold && abs_y > (threshold-getParam("ui-bottom-height")* getParam("window-height-zoomed") / getParam("window-height")) && getParam("mouse-y-position") > 0) {
		if (position.y > (0 - getParam("ui-bottom-height")* getParam("window-height-zoomed") / getParam("window-height"))) {
			position -= up * GAME::CAMERA_MOVESPEED;
		}
	}

	/* MOUSE SCROLLING --> CAMERA ZOOM */
	if (GLB::GAME && !GAME::MENU_IS_ACTIVE && GLB::CTRL_BUTTON && GLB::MOUSE_SCROLL_BOOL) {
		if (GLB::MOUSE_SCROLL > 0 && GAME::ZOOM_CURRENT > 1.0f) {
			GAME::ZOOM_CURRENT -= (int)GLB::MOUSE_SCROLL;
		}

		else if (GLB::MOUSE_SCROLL < 0 && GAME::ZOOM_CURRENT < 8.0f) {
			GAME::ZOOM_CURRENT -= (int)GLB::MOUSE_SCROLL;
		}
		GLB::MOUSE_SCROLL_BOOL = false;
		
	}

	if (position.x > GAME::MAP_WIDTH - getParam("window-width-zoomed") - GAME::CAMERA_MOVESPEED) {
		position.x = GAME::MAP_WIDTH - getParam("window-width-zoomed") - GAME::CAMERA_MOVESPEED;
	}
	if (position.y < -getParam("ui-bottom-height") * getParam("window-height-zoomed") / getParam("window-height")) {
		position.y = -getParam("ui-bottom-height") * getParam("window-height-zoomed") / getParam("window-height");
	}
	if (position.y > GAME::MAP_HEIGHT - getParam("window-height-zoomed") + getParam("ui-top-height") * getParam("window-height-zoomed") / getParam("window-height")) {
		position.y = GAME::MAP_HEIGHT - getParam("window-height-zoomed") + getParam("ui-top-height") * getParam("window-height-zoomed") / getParam("window-height");
	}
	/* ------------------------------------------- */	
	setParam("window-width-zoomed", getParam("window-width") + (GAME::ZOOM_CURRENT - 1) * GAME::ZOOM_CAMERA_FACTOR);
	setParam("window-height-zoomed", getParam("window-height") + (GAME::ZOOM_CURRENT - 1) *  GAME::ZOOM_CAMERA_FACTOR / getParam("window-ratio"));
	setParam("camera-x-position", position.x);
	setParam("camera-y-position", position.y);
}

void Camera::keyboardControl() {
	//Left margin 
	if (GLB::LEFT_KEY && position.x > 0) {
		position -= right * GAME::CAMERA_MOVESPEED;
	}
	//Right margin 
	if (GLB::RIGHT_KEY && position.x < GAME::MAP_WIDTH - getParam("window-width-zoomed")) {
		position += right * GAME::CAMERA_MOVESPEED;
	}
	//Bottom margin 
	if (GLB::UP_KEY && (position.y < (GAME::MAP_HEIGHT - getParam("window-height-zoomed") + getParam("ui-top-height")* getParam("window-height-zoomed") / getParam("window-height")))) {
		position += up * GAME::CAMERA_MOVESPEED;
	}
	//Bottom margin 
	if (GLB::DOWN_KEY && (position.y > (0 - getParam("ui-bottom-height")* getParam("window-height-zoomed") / getParam("window-height")))) {
		position -= up * GAME::CAMERA_MOVESPEED;
	}

	setParam("camera-x-position", position.x);
	setParam("camera-y-position", position.y);
}

void Camera::go_to_pos(GLfloat x, GLfloat y) {
	if (x < 0.0) { x = 0; }
	else if (x > GAME::MAP_WIDTH - getParam("window-width-zoomed")) { x = GAME::MAP_WIDTH - getParam("window-width-zoomed"); }
	if (y < 0.0) { y = 0; }
	else if (y > GAME::MAP_HEIGHT - getParam("window-height-zoomed")) { y = GAME::MAP_HEIGHT - getParam("window-height-zoomed"); }

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
