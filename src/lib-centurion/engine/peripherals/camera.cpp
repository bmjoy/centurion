#include <engine.h>

#include <game/game.h>
#include <game/interface/editorMenuBar.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

// define properties
float Engine::Camera::MaxZoom = 20.f;
float Engine::Camera::MovementSpeed = 10.f;
// ---------- end definitions

namespace Engine 
{
	namespace Camera
	{
		// Private members 
		namespace
		{
			glm::vec3 position;
			glm::vec3 front, up, right, worldUp;
			GLfloat yaw, pitch;
			float threshold_x, threshold_y, abs_x, abs_y;
			float zoomCameraFactor = 100.f;
			int currentZoom = 8;

			void update() {
				front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
				front.y = sin(glm::radians(pitch));
				front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
				front = glm::normalize(front);

				right = glm::normalize(glm::cross(front, worldUp));
				up = glm::normalize(glm::cross(right, front));
			}
		};

		int Engine::Camera::GetCurrentZoom(void)
		{
			return currentZoom;
		}

		void Engine::Camera::SetCurrentZoom(const int z)
		{
			currentZoom = z;
		}

		float Engine::Camera::GetZoomFactor(void)
		{
			return zoomCameraFactor;
		}

		void Engine::Camera::SetZoomFactor(const float f)
		{
			zoomCameraFactor = f;
		}

		float Engine::Camera::GetXPosition(void)
		{
			return position.x;
		}

		float Engine::Camera::GetYPosition(void)
		{
			return position.y;
		}

		vec2 Engine::Camera::GetZoomedCoords(const float xCoord, const float yCoord)
		{
			float x = xCoord * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
			float y = yCoord * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
			return vec2(x, y);
		}

		void Engine::Camera::Init(const glm::vec3 startPosition, const glm::vec3 startUp, const GLfloat startYaw, const GLfloat startPitch) {
			position = startPosition;

			worldUp = startUp;
			yaw = startYaw;
			pitch = startPitch;
			front = glm::vec3(0.0f, 0.0f, 1.0f);

			update();
		}

		void Engine::Camera::mouseControl(void) {
			// Conditions for NOT moving

			// 1. Block the movement if the mouse is hovering the editor menu bar
			if (GetEnvironment() == EDITOR_ENV)
			{
				if (EditorMenuBar::IsHidden() == false)
				{
					if (Mouse::GetYPosition() >= Engine::myWindow::Height - EditorMenuBar::GetHeight()) return;
					if (EditorMenuBar::IsOpened()) return;
				}
			}

			abs_x = Mouse::GetXPosition() + position.x;
			abs_y = Mouse::GetYPosition() + position.y;

			threshold_x = MEDIUM_MAP_WIDTH - 2 * MovementSpeed + (myWindow::WidthZoomed - myWindow::Width);
			threshold_y = MEDIUM_MAP_HEIGHT - 2 * MovementSpeed + (myWindow::HeightZoomed - myWindow::Height);

			float threshold_top = CAMERA_THRESHOLD;
			if (GetEnvironment() == EDITOR_ENV) threshold_top += myWindow::TopBarHeight;

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
			//!game::GameMenu::IsActive() && 
			if ((Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT_CONTROL) || Keyboard::IsKeyNotReleased(GLFW_KEY_RIGHT_CONTROL)) && Mouse::ScrollBool) {
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

		void Engine::Camera::keyboardControl(void) {
			// Conditions for NOT moving

			//Left margin
			if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT) && position.x > 0) {
				position -= right * MovementSpeed;
			}
			//Right margin 
			if (Keyboard::IsKeyPressed(GLFW_KEY_RIGHT) && position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
				position += right * MovementSpeed;
			}
			//Top margin 
			if (Keyboard::IsKeyPressed(GLFW_KEY_UP) && (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
				position += up * MovementSpeed;
			}
			//Bottom margin 
			if (Keyboard::IsKeyPressed(GLFW_KEY_DOWN) && (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
				position -= up * MovementSpeed;
			}
		}

		void Engine::Camera::GoToPoint(const GLfloat x, const GLfloat y) {
			position.x = x;
			position.y = y;
			if (position.x < 0.0) { position.x = 0; }
			else if (x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) { position.x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed; }
			if (position.y < 0.0) { position.y = 0; }
			else if (position.y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed) { position.y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed; }
		}

		glm::mat4 Engine::Camera::calculateViewMatrix(void)
		{
			return glm::lookAt(position, position + front, up);
		}

		float Engine::Camera::GetYMinimapCoordinate(const float y)
		{
			return myWindow::Height * (y - myWindow::BottomBarHeight) / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
		}

	};
};



