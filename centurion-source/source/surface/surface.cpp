#include <surface>
#include <global>
#include <engine>
#include <game>

using namespace glb;
using namespace game;

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

	/*if (getBoolean("mouse-left")) {
		float x1 = (getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position"));
		float y1 = (getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position"));
		obj::MapTerrain()->updateBuffers(x1, y1, "terrain", 2.f);
		obj::MapTerrain()->updateBuffers(x1, y1, "zNoise", 10.f);
	}*/

	obj::MapTerrain()->render(tracing);

	if (gameGridStatus && !tracing) {
		obj::MapGrid()->render();
	}
}

Surface::~Surface()
{
}
