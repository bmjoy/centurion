#include <engine>
#include <stb_image.h>
#include <surface>

using namespace glb;

Mouse::Mouse(){
	currentState = "default";
}
void Mouse::create() {
	img = gui::Image("circle_pos");
	img.create("center", 0.f, 0.f, 0, 0, 0);
}
void Mouse::render() {
	obj::Cursor()->render(position.x, position.y, currentState);
	if (GLB::GAME){
		if (!GAME::MINIMAP_IS_ACTIVE) {
			img.render(false, getParam("mouse-x-position"), getParam("mouse-y-2D-position"));
		}
	}
}
void Mouse::mouse_control(int lastX, int lastY) {

	position.x = (GLfloat)lastX;
	position.y = (GLfloat)lastY;

	setParam("mouse-x-position", position.x);
	setParam("mouse-y-position", position.y);
	
	yzoomed = getZoomedCoords(getParam("mouse-x-position"), getParam("mouse-y-position")).y;
	znoise = mapgen::smoothNoise(yzoomed, GLB::Z_NOISE);
	znoise /= getParam("window-height-zoomed") / getParam("window-height");

	setParam("mouse-y-2D-position", getParam("mouse-y-position") - znoise);
	
	if (!GLB::MOUSE_LEFT) {
		setParam("mouse-x-leftclick", getParam("mouse-x-position"));
		setParam("mouse-y-leftclick", getParam("mouse-y-position"));
		
	}
	else {
		currentState = "left";
		printf("DEBUG: You have left-clicked on (X=%d, Y=%d)\n", (int)getParam("mouse-x-leftclick"), (int)getParam("mouse-y-leftclick"));
	}

	if (!GLB::MOUSE_RIGHT) {
		setParam("mouse-x-rightclick", getParam("mouse-x-position"));
		setParam("mouse-y-rightclick", getParam("mouse-y-position"));
		setParam("mouse-y-2D-rightclick", getParam("mouse-y-2D-position"));
	}
	else {
		currentState = "right";
		printf("DEBUG: You have right-clicked on (X=%d, Y=%d)\n", (int)getParam("mouse-x-rightclick"), (int)getParam("mouse-y-rightclick"));
	}

	if (GLB::MOUSE_RELEASE) {
		currentState = "default";
	}
}

Mouse::~Mouse(){}
