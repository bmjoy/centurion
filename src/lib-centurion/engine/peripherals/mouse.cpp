#include <engine.h>

#include <ui.h>
#include <game/game.h>
#include <mapgen/mapgen.h>
#include <logger.h>
#include <Settings.h>
#include <cursor_image.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

// DEFINE PROPERTIES
float Engine::Mouse::ScrollValue = 0;
bool Engine::Mouse::ScrollBool = false;
bool Engine::Mouse::LeftClick = false;
bool Engine::Mouse::RightClick = false;
bool Engine::Mouse::MiddleClick = false;
bool Engine::Mouse::LeftHold = false;
bool Engine::Mouse::RightHold = false;
bool Engine::Mouse::MiddleHold = false;
bool Engine::Mouse::Release = false;

namespace Engine
{
	namespace Mouse
	{

		// Private variables 
		namespace
		{
			struct HoldClickData
			{
				double lastTime = 0;
				bool bIsTimeSaved = false;
			} leftHoldClickData, rightHoldClickData, middleHoldClickData;

			gui::Image img = gui::Image();
			glm::vec3 position = glm::vec3();
			int currentState = CURSOR_DEFAULT;
			float znoise = 0, yzoomed = 0, xPosGrid = 0, yPosGrid = 0, xLeftClick = 0, yLeftClick = 0, xRightClick = 0, yRightClick = 0, y2DPosition = 0, y2DRightClick = 0;

		};

		void Engine::Mouse::Create(void) {
			img = gui::Image("circle_pos");
			img.create("center", 0.f, 0.f, 0, 0, 0);
		}

		void Render(void) {
			Cursor()->render(position.x, position.y, currentState);

			if (Engine::GetEnvironment() == STRATEGY_ENV) {
				if (Game::Minimap::IsActive() == false) {
					img.render(false, position.x, y2DPosition);
				}
			}
		}

		float Engine::Mouse::GetXMapCoordinate(void)
		{
			return round(Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
		}

		float Engine::Mouse::GetYMapCoordinate(void)
		{
			return round(Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
		}

		bool Engine::Mouse::IsCursorInGameScreen(void)
		{
			return (GetYLeftClick() > myWindow::BottomBarHeight) && (GetYLeftClick() < (myWindow::Height - myWindow::TopBarHeight));
		}

		float Engine::Mouse::GetXPosition(void)
		{
			return position.x;
		}

		float Engine::Mouse::GetYPosition(void)
		{
			return position.y;
		}

		float Engine::Mouse::GetY2DPosition(void)
		{
			return y2DPosition;
		}

		float Engine::Mouse::GetXLeftClick(void)
		{
			return xLeftClick;
		}

		float Engine::Mouse::GetYLeftClick(void)
		{
			return yLeftClick;
		}

		float Engine::Mouse::GetXRightClick(void)
		{
			return xRightClick;
		}

		float Engine::Mouse::GetYRightClick(void)
		{
			return yRightClick;
		}

		float Engine::Mouse::GetY2DRightClick(void)
		{
			return y2DRightClick;
		}

		void Engine::Mouse::Control(const int lastX, const int lastY) {
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
				currentState = CURSOR_LEFT_CLICK;

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
				currentState = CURSOR_RIGHT_CLICK;

				stringstream ss;
				ss << "You have right-clicked on (X=" << (int)xRightClick << ", Y=" << (int)yRightClick << ")";
				Logger::Info(ss.str());
			}

			if (Mouse::Release) {
				currentState = CURSOR_DEFAULT;
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

		void Engine::Mouse::Input(const int button, const int action)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT) {
				if (action == GLFW_PRESS) {
					Mouse::LeftClick = true;
					if (Mouse::leftHoldClickData.bIsTimeSaved == false) {
						Mouse::leftHoldClickData.lastTime = glfwGetTime();
						Mouse::leftHoldClickData.bIsTimeSaved = true;
					}
				}
				else if (action == GLFW_RELEASE) {
					Mouse::Release = true;
					if (Mouse::leftHoldClickData.bIsTimeSaved) {
						Mouse::leftHoldClickData.bIsTimeSaved = false;
						Mouse::LeftHold = false;
					}
				}
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT) {
				if (action == GLFW_PRESS) {
					Mouse::RightClick = true;
					if (Mouse::rightHoldClickData.bIsTimeSaved == false) {
						Mouse::rightHoldClickData.lastTime = glfwGetTime();
						Mouse::rightHoldClickData.bIsTimeSaved = true;
					}
				}
				else if (action == GLFW_RELEASE) {
					Mouse::Release = true;
					if (Mouse::rightHoldClickData.bIsTimeSaved) {
						Mouse::rightHoldClickData.bIsTimeSaved = false;
						Mouse::RightHold = false;
					}
				}
			}
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
				if (action == GLFW_PRESS) {
					Mouse::MiddleClick = true;
					if (Mouse::middleHoldClickData.bIsTimeSaved == false) {
						Mouse::middleHoldClickData.lastTime = glfwGetTime();
						Mouse::middleHoldClickData.bIsTimeSaved = true;
					}
				}
				else if (action == GLFW_RELEASE) {
					Mouse::MiddleClick = false;
					if (Mouse::middleHoldClickData.bIsTimeSaved) {
						Mouse::middleHoldClickData.bIsTimeSaved = false;
						Mouse::MiddleHold = false;
					}
				}
			}
		}
	};
};
