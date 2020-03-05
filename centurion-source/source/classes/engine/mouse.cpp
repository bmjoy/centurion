#include <stb_image.h>
#include <surface>
#include "mouse.h"
#include "engine.h"
#include "window.h"
#include <game/strategy.h>

using namespace glb;


namespace engine {

	// define static variables (default values)
	gui::Image Mouse::img;
	vec3 Mouse::position;
	string Mouse::currentState = "default";
	float Mouse::znoise = 0;
	float Mouse::yzoomed = 0;
	float Mouse::xPosGrid = 0;
	float Mouse::yPosGrid = 0;
	float Mouse::xLeftClick = 0;
	float Mouse::yLeftClick = 0;
	float Mouse::xRightClick = 0;
	float Mouse::yRightClick = 0;
	float Mouse::y2DRightClick = 0;
	float Mouse::y2DPosition = 0;
	float Mouse::ScrollValue = 0;
	bool Mouse::ScrollBool = false;
	bool Mouse::LeftClick = false;
	bool Mouse::RightClick = false;
	bool Mouse::LeftHold = false;
	bool Mouse::Release = false;
	bool Mouse::MiddleClick = false;
	// ------------ end definitions

	Mouse::Mouse() { }
	
	void Mouse::create() {
		img = gui::Image("circle_pos");
		img.create("center", 0.f, 0.f, 0, 0, 0);
	}
	void Mouse::render() {
		obj::Cursor()->render(position.x, position.y, currentState);
		if (Engine::getEnvironment() == "game") {
			if (Game::Minimap::IsActive() == false) {
				img.render(false, position.x, y2DPosition);
			}
		}
	}
	void Mouse::mouse_control(int lastX, int lastY) {
		position.x = (GLfloat)lastX;
		position.y = (GLfloat)lastY;

		yzoomed = getZoomedCoords(position.x, Mouse::GetYPosition()).y;
		znoise = mapgen::smoothNoise(yzoomed, mapgen::mouseZNoise);
		znoise /= myWindow::HeightZoomed / myWindow::Height;

		y2DPosition = Mouse::GetYPosition() - znoise;

		if (!LeftClick) {
			xLeftClick = position.x;
			yLeftClick = position.y;
		}
		else {
			currentState = "left";
			printf("[DEBUG] You have left-clicked on (X=%d, Y=%d)\n", (int)xLeftClick, (int)yLeftClick);
		}

		if (!RightClick) {
			xRightClick = position.x;
			yRightClick = position.y;
			y2DRightClick = y2DPosition;
		}
		else {
			currentState = "right";
			printf("[DEBUG] You have right-clicked on (X=%d, Y=%d)\n", (int)xRightClick, (int)yRightClick);
		}

		if (Mouse::Release) {
			currentState = "default";
		}
	}

	Mouse::~Mouse() {}

}