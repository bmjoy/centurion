#include "surface.h"
#include <mapgen>

using namespace glb;

Surface::Surface()
{	
}

void Surface::createNoise() {
	mapgen::generateRandomMap();
	obj::MapTerrain()->updateHeightsBuffer();
}

void Surface::updateGrid() {
	obj::MapGrid()->update();
}

void Surface::render(bool tracing) {

	if (GLB::MOUSE_LEFT && GLB::EDITOR) {
		float x1 = (GLB::MOUSE_X * getParam("window-width-zoomed") / getParam("window-width") + GAME::CAMERA_POS_X);
		float y1 = (GLB::MOUSE_Y * getParam("window-height-zoomed") / getParam("window-height") + GAME::CAMERA_POS_Y);
		//obj::MapTerrain()->updateBuffers(x1, y1, "terrain", 2.f);
		//obj::MapTerrain()->updateBuffers(x1, y1, "zNoise", 10.f);
	}

	obj::MapTerrain()->render(tracing);

	if (GAME::GRID_IS_ACTIVE && !tracing) {
		obj::MapGrid()->render();
	}
}

Surface::~Surface()
{
}
