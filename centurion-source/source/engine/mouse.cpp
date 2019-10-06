#include "stb_image.h"  // manip. texture
#include "mouse.h"

using namespace glb;

Mouse::Mouse(){
	currentState = "default";
}

void Mouse::render() {
	obj::Cursor()->render(position.x, position.y, currentState);
}

void Mouse::mouse_control(int lastX, int lastY) {

	position.x = (GLfloat)lastX;
	position.y = (GLfloat)lastY;

	GLB::MOUSE_X = (int)position.x;
	GLB::MOUSE_Y = (int)position.y;
	
	yzoomed = getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y;
	znoise = mapgen::smoothNoise(yzoomed, GLB::Z_NOISE);
	znoise /= getParam("window-height-zoomed") / getParam("window-height");

	GLB::MOUSE_Y_2D = (GLB::MOUSE_Y - (int)znoise);
	
	if (!GLB::MOUSE_LEFT) {
		GLB::MOUSE_LEFT_X = (int)lastX;
		GLB::MOUSE_LEFT_Y = (int)lastY;
		
	}
	else {
		currentState = "left";
		printf("DEBUG: You have left-clicked on (X=%d, Y=%d)\n", GLB::MOUSE_LEFT_X, GLB::MOUSE_LEFT_Y);
	}

	if (!GLB::MOUSE_RIGHT) {
		GLB::MOUSE_RIGHT_X = GLB::MOUSE_X;
		GLB::MOUSE_RIGHT_Y = GLB::MOUSE_Y;
		GLB::MOUSE_RIGHT_Y_2D = GLB::MOUSE_Y_2D;
	}
	else {
		currentState = "right";
		printf("DEBUG: You have right-clicked on (X=%d, Y=%d)\n", GLB::MOUSE_RIGHT_X, GLB::MOUSE_RIGHT_Y);
	}

	if (GLB::MOUSE_RELEASE) {
		currentState = "default";
	}
}

Mouse::~Mouse(){}
