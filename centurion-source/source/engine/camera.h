#pragma once

#include <global.hpp>

class Camera
{
public:
	Camera();
	glm::vec3 position;
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch);
	void mouseControl(float threshold);
	void keyboardControl();
	void go_to_pos(GLfloat x, GLfloat y);
	glm::mat4 calculateViewMatrix();
	~Camera();

private:
	
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	float threshold_x, threshold_y, abs_x, abs_y;
	void update();
};

