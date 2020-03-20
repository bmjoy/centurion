#include <engine.h>

#include <game/game.h>
#include <mapgen/mapgen.h>

#include <logger.h>
#include <Settings.h>

// for mouse cursor
#include <cursor_image.h>

#include <GLFW/glfw3.h>

// define static variables (default values)
gui::Image Engine::Mouse::img;
vec3 Engine::Mouse::position;
string Engine::Mouse::currentState = "default";
float Engine::Mouse::znoise = 0;
float Engine::Mouse::yzoomed = 0;
float Engine::Mouse::xPosGrid = 0;
float Engine::Mouse::yPosGrid = 0;
float Engine::Mouse::xLeftClick = 0;
float Engine::Mouse::yLeftClick = 0;
float Engine::Mouse::xRightClick = 0;
float Engine::Mouse::yRightClick = 0;
float Engine::Mouse::y2DRightClick = 0;
float Engine::Mouse::y2DPosition = 0;
float Engine::Mouse::ScrollValue = 0;
bool Engine::Mouse::ScrollBool = false;
bool Engine::Mouse::LeftClick = false;
bool Engine::Mouse::RightClick = false;
bool Engine::Mouse::MiddleClick = false;
bool Engine::Mouse::LeftHold = false;
bool Engine::Mouse::RightHold = false;
bool Engine::Mouse::MiddleHold = false;
bool Engine::Mouse::Release = false;
Engine::HoldClickData Engine::Mouse::leftHoldClickData;
Engine::HoldClickData Engine::Mouse::rightHoldClickData;
Engine::HoldClickData Engine::Mouse::middleHoldClickData;
// ------------ end definitions

Engine::Mouse::Mouse() { }

bool Engine::Mouse::IsCursorInGameScreen()
{
	return (GetYLeftClick() > myWindow::BottomBarHeight) && (GetYLeftClick() < (myWindow::Height - myWindow::TopBarHeight));
}

void Engine::Mouse::create() {
	img = gui::Image("circle_pos");
	img.create("center", 0.f, 0.f, 0, 0, 0);
}
void Engine::Mouse::render() {
	Cursor()->render(position.x, position.y, currentState);
	if (Engine::getEnvironment() == STRATEGY_ENV) {
		if (Game::Minimap::IsActive() == false) {
			img.render(false, position.x, y2DPosition);
		}
	}
}
void Engine::Mouse::mouse_control(int lastX, int lastY) {
	position.x = (GLfloat)lastX;
	position.y = (GLfloat)lastY;

	yzoomed = Camera::GetZoomedCoords(position.x, Mouse::GetYPosition()).y;
	znoise = mapgen::smoothNoise(yzoomed, mapgen::mouseZNoise);
	znoise /= myWindow::HeightZoomed / myWindow::Height;

	y2DPosition = Mouse::GetYPosition() - znoise;

	if (!LeftClick) {
		xLeftClick = position.x;
		yLeftClick = position.y;
	}
	else {
		currentState = "left";

		stringstream ss;
		ss << "You have left-clicked on (X=" << (int)xLeftClick << ", Y=" << (int)yLeftClick << ")";
		Logger::Info(ss.str());
	}

	if (!RightClick) {
		xRightClick = position.x;
		yRightClick = position.y;
		y2DRightClick = y2DPosition;
	}
	else {
		currentState = "right";

		stringstream ss;
		ss << "You have right-clicked on (X=" << (int)xRightClick << ", Y=" << (int)yRightClick << ")";
	}

	if (Mouse::Release) {
		currentState = "default";
	}
}

void Engine::Mouse::IsHolding(void)
{
	if (glfwGetTime() - leftHoldClickData.lastTime > TIME_LEFT_HOLD && leftHoldClickData.bIsTimeSaved) {
		LeftHold = true;
	}
	if (glfwGetTime() - rightHoldClickData.lastTime > TIME_LEFT_HOLD && rightHoldClickData.bIsTimeSaved) {
		RightHold = true;
	}
	if (glfwGetTime() - middleHoldClickData.lastTime > TIME_LEFT_HOLD && middleHoldClickData.bIsTimeSaved) {
		MiddleHold = true;
	}
}

Engine::Mouse::~Mouse() {}
