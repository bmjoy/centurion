#include "game.h"
#include <surface>
#include <global>
#include "strategy.h"

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






void Game::RenderObjectsPicking() {
	/*if (leftClickID_UI != 0) {
		leftClickID = 0;
		rightClickID = 0;
		return;
	}

	if ((Mouse::RightClick || Mouse::LeftClick) && !selRectangleIsActive) {
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			bld->second.render(true, 0);
		}
		for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
			u->second.render(true, 0);
		}

		if (Mouse::LeftClick) leftClickID = get_id("left");
		if (Mouse::RightClick) rightClickID = get_id("right");

		if (gameMinimapStatus) {
			blockMinimap = false;
			if (leftClickID > 0) {
				blockMinimap = true;
			}
		}
	}*/
}

