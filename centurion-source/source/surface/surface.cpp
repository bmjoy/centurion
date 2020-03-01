#include <surface>
#include <global>
#include <game>

using namespace glb;
using namespace game;

bool Surface::Wireframe = false;

Surface::Surface(){}

void Surface::reset() {
	mapgen::reset_map();
	obj::MapGrid()->reset();
	obj::MapTerrain()->updateHeightsBuffer();
	obj::MapTerrain()->updateTextureBuffer();
}

void Surface::createNoise() {
	mapgen::generateRandomMap();
	obj::MapTerrain()->updateHeightsBuffer();

	cout << "[DEBUG] Terrain has been generated!\n";
	cout << "[DEBUG] Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ << endl;
}

void Surface::updateGrid() {
	obj::MapGrid()->update();
}

void Surface::render(bool tracing) {

	obj::MapTerrain()->render(tracing);

	if (gameGridStatus && !tracing) {
		obj::MapGrid()->render();
	}
}

Surface::~Surface()
{
}
