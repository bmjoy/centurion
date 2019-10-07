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
		float x1 = (getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position"));
		float y1 = (getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position"));
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
