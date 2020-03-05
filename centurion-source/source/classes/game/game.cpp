#include "game.h"
#include <surface>
#include <global>
#include "strategy.h"
#include <picking>
#include <engine/mouse.h>
#include <engine/window.h>
#include <engine/camera.h>
#include <interface>
#include "editor.h"

#pragma region Namespaces

using namespace glb;
using namespace engine;

#pragma endregion

#pragma region Static variables

#pragma endregion

#pragma region Surface

bool Game::Surface::Wireframe = false;

Game::Surface::Surface() {}

void Game::Surface::Reset() {
	mapgen::reset_map();
	obj::MapGrid()->reset();
	obj::MapTerrain()->updateHeightsBuffer();
	obj::MapTerrain()->updateTextureBuffer();
}

void Game::Surface::CreateNoise() {
	mapgen::generateRandomMap();
	obj::MapTerrain()->updateHeightsBuffer();

	cout << "[DEBUG] Terrain has been generated!\n";
	cout << "[DEBUG] Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ << endl;
}

void Game::Surface::UpdateGrid() {
	obj::MapGrid()->update();
}

void Game::Surface::Render(bool tracing) {

	obj::MapTerrain()->render(tracing);

	if (gameGridStatus && !tracing) {
		obj::MapGrid()->render();
	}
}

Game::Surface::~Surface() {}

#pragma endregion

#pragma region Selection rectangle

Game::SelectionRectangle::SelRectPoints Game::SelectionRectangle::Coordinates;
float Game::SelectionRectangle::cameraLastX = 0.f;
float Game::SelectionRectangle::cameraLastY = 0.f;
bool Game::SelectionRectangle::isActive = false;
gui::Rectangle Game::SelectionRectangle::selRectangle;

Game::SelectionRectangle::SelectionRectangle() {}

void Game::SelectionRectangle::Create() {
	cameraLastX = 0.f;
	cameraLastY = 0.f;
	selRectangle = gui::Rectangle();
	selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
}

bool Game::SelectionRectangle::IsInRectangle(array<float, 8> &coords) {
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

void Game::SelectionRectangle::Render(){
	if (Mouse::LeftHold) {
		
		if (SelectionRectangle::IsActive() == false) {
			cout << "[DEBUG] Selection rectangle enabled.\n";
			Coordinates.startX = Mouse::GetXLeftClick() * myWindow::WidthZoomed / myWindow::Width + cameraLastX;
			Coordinates.startY = Mouse::GetYLeftClick() * myWindow::HeightZoomed / myWindow::Height + cameraLastY;
		}
		Coordinates.lastX = Mouse::GetXPosition() * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
		Coordinates.lastY = Mouse::GetYPosition() * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
		if (Mouse::GetYPosition() < myWindow::BottomBarHeight) {
			Coordinates.lastY = myWindow::BottomBarHeight*myWindow::HeightZoomed / myWindow::Height + 1.0f + Camera::GetYPosition();
		}
		if (Mouse::GetYPosition() > myWindow::Height - myWindow::TopBarHeight) {
			Coordinates.lastY = myWindow::HeightZoomed - myWindow::TopBarHeight*myWindow::HeightZoomed / myWindow::Height - 1.0f + Camera::GetYPosition();
		}

		float w = (Coordinates.lastX - Coordinates.startX);
		float h = (Coordinates.lastY - Coordinates.startY);

		int origin = 0;
		if (w > 0 && h > 0) origin = 0; // bottom-left
		if (w > 0 && h < 0) origin = 1; // top-left
		if (w < 0 && h > 0) origin = 4; // bottom-right
		if (w < 0 && h < 0) origin = 3; // top-right

		if (abs(w) > 1 && abs(h) > 1) {
			selRectangle.render(vec4(255.f), 0, Coordinates.startX, Coordinates.startY, abs(w), abs(h), origin);
			Coordinates.minX = std::min(Coordinates.startX, Coordinates.lastX);
			Coordinates.maxX = std::max(Coordinates.startX, Coordinates.lastX);
			Coordinates.minY = std::min(Coordinates.startY, Coordinates.lastY);
			Coordinates.maxY = std::max(Coordinates.startY, Coordinates.lastY);
		}
		else {
			Coordinates.minX = -0.1f;
			Coordinates.maxX = -0.1f;
			Coordinates.minY = -0.1f;
			Coordinates.maxY = -0.1f;
		}
		SelectionRectangle::Enable();
	}
	else {
		if (SelectionRectangle::IsActive()) cout << "[DEBUG] Selection rectangle disabled.\n";
		cameraLastX = Camera::GetXPosition();
		cameraLastY = Camera::GetYPosition();
		Coordinates.startX = -0.1f;
		Coordinates.startY = -0.1f;
		Coordinates.lastX = -0.1f;
		Coordinates.lastY = -0.1f;
		Coordinates.minX = -0.1f;
		Coordinates.maxX = -0.1f;
		Coordinates.minY = -0.1f;
		Coordinates.maxY = -0.1f;
		SelectionRectangle::Disable();
	}
}

#pragma endregion

#pragma region Minimap prerendered

bool Game::Minimap::isCreated = false;
bool Game::Minimap::isActive = false;
bool Game::Minimap::isBlocked = false;

Game::Minimap::Minimap() {}

void Game::Minimap::Create() {
	obj::MMRectangle()->update();
	isCreated = true;
}

void Game::Minimap::Render() {
	obj::MMRectangle()->render();
}

Game::Minimap::~Minimap() {}

#pragma endregion



Game::Game(){}
Game::~Game(){}


void Game::RenderObjectsPicking() {
	if (leftClickID_UI != 0) {
		leftClickID = 0;
		rightClickID = 0;
		return;
	}

	if ((Mouse::RightClick || Mouse::LeftClick) && !SelectionRectangle::IsActive()) {
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			bld->second.render(true, 0);
		}
		for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
			u->second.render(true, 0);
		}

		if (Mouse::LeftClick) leftClickID = get_id("left");
		if (Mouse::RightClick) rightClickID = get_id("right");

		if (Minimap::IsActive()) {
			Minimap::Unblock();
			if (leftClickID > 0) {
				Minimap::Block();
			}
		}
	}
}

void Game::RenderObjects() {
	int selectedBuildings = 0;
	for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
		bld->second.render(false, leftClickID);
		if (bld->second.isSelected()) selectedBuildings++;
	}
	if (selectedBuildings == 0) game::GAME_UI()->set_ui(nullptr);

	for (map<int, Decoration>::iterator dec = decorations.begin(); dec != decorations.end(); dec++) {
		dec->second.render();
	}
	if (Minimap::IsActive() == false) {
		for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
			u->second.render(false, leftClickID);
			if (u->second.isSelected())	  selectedUnits++;
		}
	}
	/*cout << gameMinimapStatus << " " << editor::IsWindowOpened << " " << editor::menuIsOpened << " " << editor::TerrainBrushIsActive << " " << leftClickID_UI << editor::movingObject << endl;*/
	if (!Minimap::IsActive() && !editor::IsWindowOpened && !editor::menuIsOpened && !editor::TerrainBrushIsActive && leftClickID_UI == 0 && !editor::movingObject) SelectionRectangle::Render();
}
