#include "camera.h"


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

	abs_x = GLB::MOUSE_X + position.x;
	abs_y = GLB::MOUSE_Y + position.y;

	threshold_x = GAME::MAP_WIDTH - 2*GAME::CAMERA_MOVESPEED + (GLB::WINDOW_WIDTH_ZOOMED - GLB::WINDOW_WIDTH);
	threshold_y = GAME::MAP_HEIGHT - 2*GAME::CAMERA_MOVESPEED + (GLB::WINDOW_HEIGHT_ZOOMED - GLB::WINDOW_HEIGHT);

	//Left margin
	if (GLB::MOUSE_X <= threshold && (abs_x > threshold) && GLB::MOUSE_X > 0) {
		position -= right * GAME::CAMERA_MOVESPEED;
	}
	//Right margin
	if (GLB::MOUSE_X >= (GLB::WINDOW_WIDTH - threshold) && (abs_x < threshold_x) && GLB::MOUSE_X < GLB::WINDOW_WIDTH) {
		if (position.x < GAME::MAP_WIDTH - GLB::WINDOW_WIDTH_ZOOMED){
			position += right * GAME::CAMERA_MOVESPEED;
		}
	}
	//Top margin
	if (GLB::MOUSE_Y >= (GLB::WINDOW_HEIGHT - threshold) && abs_y < (threshold_y + GAME::UI_TOP_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT) && GLB::MOUSE_Y < GLB::WINDOW_HEIGHT) {
		if (position.y < (GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT_ZOOMED + GAME::UI_TOP_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT)) {
			position += up * GAME::CAMERA_MOVESPEED;
		}
	}
	//Bottom margin
	if (GLB::MOUSE_Y <= threshold && abs_y > (threshold-GAME::UI_BOTTOM_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT) && GLB::MOUSE_Y > 0) {
		if (position.y > (0 - GAME::UI_BOTTOM_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT)) {
			position -= up * GAME::CAMERA_MOVESPEED;
		}
	}

	/* MOUSE SCROLLING --> CAMERA ZOOM */
	if (GLB::GAME && GLB::CTRL_BUTTON && GLB::MOUSE_SCROLL_BOOL) {
		if (GLB::MOUSE_SCROLL > 0 && GAME::ZOOM_CURRENT > 1.0f) {
			GAME::ZOOM_CURRENT -= (int)GLB::MOUSE_SCROLL;
		}

		else if (GLB::MOUSE_SCROLL < 0 && GAME::ZOOM_CURRENT < 8.0f) {
			GAME::ZOOM_CURRENT -= (int)GLB::MOUSE_SCROLL;
		}
		GLB::MOUSE_SCROLL_BOOL = false;
		
	}

	if (position.x > GAME::MAP_WIDTH - GLB::WINDOW_WIDTH_ZOOMED - GAME::CAMERA_MOVESPEED) {
		position.x = GAME::MAP_WIDTH - GLB::WINDOW_WIDTH_ZOOMED - GAME::CAMERA_MOVESPEED;
	}
	if (position.y < -GAME::UI_BOTTOM_HEIGHT * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT) {
		position.y = -GAME::UI_BOTTOM_HEIGHT * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT;
	}
	if (position.y > GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT_ZOOMED + GAME::UI_TOP_HEIGHT * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT) {
		position.y = GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT_ZOOMED + GAME::UI_TOP_HEIGHT * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT;
	}
	/* ------------------------------------------- */	
	GLB::WINDOW_WIDTH_ZOOMED = (int)((float)GLB::WINDOW_WIDTH + (GAME::ZOOM_CURRENT - 1) * GAME::ZOOM_CAMERA_FACTOR);
	GLB::WINDOW_HEIGHT_ZOOMED = (int)((float)GLB::WINDOW_HEIGHT + (GAME::ZOOM_CURRENT - 1) *  GAME::ZOOM_CAMERA_FACTOR * GLB::WINDOW_HEIGHT / GLB::WINDOW_WIDTH);
	GAME::CAMERA_POS_X = position.x;
	GAME::CAMERA_POS_Y = position.y;
}

void Camera::keyboardControl() {
	//Left margin 
	if (GLB::LEFT_KEY && position.x > 0) {
		position -= right * GAME::CAMERA_MOVESPEED;
	}
	//Right margin 
	if (GLB::RIGHT_KEY && position.x < GAME::MAP_WIDTH - GLB::WINDOW_WIDTH_ZOOMED) {
		position += right * GAME::CAMERA_MOVESPEED;
	}
	//Bottom margin 
	if (GLB::UP_KEY && (position.y < (GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT_ZOOMED + GAME::UI_TOP_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT))) {
		position += up * GAME::CAMERA_MOVESPEED;
	}
	//Bottom margin 
	if (GLB::DOWN_KEY && (position.y > (0 - GAME::UI_BOTTOM_HEIGHT* GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT))) {
		position -= up * GAME::CAMERA_MOVESPEED;
	}

	GAME::CAMERA_POS_X = position.x;
	GAME::CAMERA_POS_Y = position.y;
}

void Camera::go_to_pos(GLfloat x, GLfloat y) {
	if (x < 0.0) { x = 0; }
	else if (x > GAME::MAP_WIDTH - GLB::WINDOW_WIDTH) { x = GAME::MAP_WIDTH - GLB::WINDOW_WIDTH_ZOOMED; }
	if (y < 0.0) { y = 0; }
	else if (y > GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT) { y = GAME::MAP_HEIGHT - GLB::WINDOW_HEIGHT_ZOOMED; }

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
