#include "selection_rectangle.h"

#include "editor.h"
#include <engine.h>
#include <logger.h>
#include <ui.h>

using namespace std;
using namespace glm;

namespace Game 
{
	namespace SelectionRectangle
	{
		// Private members
		namespace
		{
			struct SelRectPoints
			{
				float startX, startY, lastX, lastY;
				float minX, maxX, minY, maxY;
			} Coordinates;

			float cameraLastX = 0, cameraLastY = 0;
			gui::Rectangle selRectangle = gui::Rectangle();
			bool isActive = false;
		};

		void Game::SelectionRectangle::Disable(void)
		{
			SelectionRectangle::isActive = false;
		}

		void Game::SelectionRectangle::ResetExtremeValues(void)
		{
			Coordinates.minX = -0.1f;
			Coordinates.maxX = -0.1f;
			Coordinates.minY = -0.1f;
			Coordinates.maxY = -0.1f;
		}

		void Game::SelectionRectangle::SetExtremeValues(void)
		{
			Coordinates.minX = std::min(Coordinates.startX, Coordinates.lastX);
			Coordinates.maxX = std::max(Coordinates.startX, Coordinates.lastX);
			Coordinates.minY = std::min(Coordinates.startY, Coordinates.lastY);
			Coordinates.maxY = std::max(Coordinates.startY, Coordinates.lastY);
		}

		void Game::SelectionRectangle::CalculateSizeAndOrigin(float * width, float * height, int * origin)
		{
			float _width = Coordinates.lastX - Coordinates.startX;
			float _height = Coordinates.lastY - Coordinates.startY;

			int _origin = 0;
			if (_width < 0 && _height < 0) _origin = TOPRIGHT_ORIGIN;
			else if (_width > 0 && _height > 0) _origin = BOTTOMLEFT_ORIGIN;
			else if (_width > 0 && _height < 0) _origin = TOPLEFT_ORIGIN;
			else _origin = BOTTOMRIGHT_ORIGIN;

			(*width) = std::abs(_width);
			(*height) = std::abs(_height);
			(*origin) = _origin;
		}

		void Game::SelectionRectangle::Create(void)
		{
			cameraLastX = 0.f;
			cameraLastY = 0.f;
			selRectangle = gui::Rectangle();
			selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
		}

		bool Game::SelectionRectangle::IsInRectangle(array<float, 8> &coords) {
			if (IsActive() == false) return false;
			return(
				// are the 4 points in selection rectangle ?
				(coords[0] > Coordinates.minX && coords[0] < Coordinates.maxX &&
					coords[1] > Coordinates.minY && coords[1] < Coordinates.maxY) ||
					(coords[2] > Coordinates.minX && coords[2] < Coordinates.maxX &&
						coords[3] > Coordinates.minY && coords[3] < Coordinates.maxY) ||
						(coords[4] > Coordinates.minX && coords[4] < Coordinates.maxX &&
							coords[5] > Coordinates.minY && coords[5] < Coordinates.maxY) ||
							(coords[6] > Coordinates.minX && coords[6] < Coordinates.maxX &&
								coords[7] > Coordinates.minY && coords[7] < Coordinates.maxY) ||
				// does the sel. rectangle and hitbox rectangle intersect themselves?
								(coords[0] > Coordinates.minX && coords[0] < Coordinates.maxX &&
									coords[3] < Coordinates.minY && coords[1] > Coordinates.maxY) ||
									(coords[4] > Coordinates.minX && coords[4] < Coordinates.maxX &&
										coords[3] < Coordinates.minY && coords[1] > Coordinates.maxY) ||
										(coords[1] > Coordinates.minY && coords[1] < Coordinates.maxY &&
											coords[0] < Coordinates.minX && coords[4] > Coordinates.maxX) ||
											(coords[3] > Coordinates.minY && coords[3] < Coordinates.maxY &&
												coords[0] < Coordinates.minX && coords[4] > Coordinates.maxX)
				);
		}

		void Game::SelectionRectangle::Render(void)
		{
			if (Editor::IsMovingObject()) return;
			if (Editor::IsChangingTerrain()) return;

			if (Engine::Mouse::LeftClick) {
				if (SelectionRectangle::IsActive() == false) {
					Logger::Info("Selection rectangle enabled.");
					Coordinates.startX = Engine::Mouse::GetXLeftClick() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + cameraLastX;
					Coordinates.startY = Engine::Mouse::GetYLeftClick() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + cameraLastY;
				}
			}
			if (Engine::Mouse::LeftHold) {
				Coordinates.lastX = Engine::Mouse::GetXMapCoordinate();
				Coordinates.lastY = Engine::Mouse::GetYMapCoordinate();
				if (Engine::Mouse::GetYPosition() < Engine::myWindow::BottomBarHeight) {
					Coordinates.lastY = Engine::myWindow::BottomBarHeight * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + 1.0f + Engine::Camera::GetYPosition();
				}
				if (Engine::Mouse::GetYPosition() > Engine::myWindow::Height - Engine::myWindow::TopBarHeight) {
					Coordinates.lastY = Engine::myWindow::HeightZoomed - Engine::myWindow::TopBarHeight*Engine::myWindow::HeightZoomed / Engine::myWindow::Height - 1.0f + Engine::Camera::GetYPosition();
				}

				float w, h;
				int origin;
				SelectionRectangle::CalculateSizeAndOrigin(&w, &h, &origin);

				if (w > 1 && h > 1) {
					selRectangle.render(vec4(), vec4(255.f), 0, 0, Coordinates.startX, Coordinates.startY, w, h, origin);
					SetExtremeValues();
				}
				else {
					SelectionRectangle::ResetExtremeValues();
				}
				SelectionRectangle::Enable();
			}
			else {
				if (SelectionRectangle::IsActive()) Logger::Info("Selection rectangle disabled.");
				cameraLastX = Engine::Camera::GetXPosition();
				cameraLastY = Engine::Camera::GetYPosition();
				SelectionRectangle::ResetExtremeValues();
				SelectionRectangle::Disable();
			}
		}

		bool Game::SelectionRectangle::IsActive(void)
		{
			return SelectionRectangle::isActive;
		}

		void Game::SelectionRectangle::Enable(void)
		{
			SelectionRectangle::isActive = true;
		}
	};
};

