#include "surface.h"

Surface::Surface()
{	
}

void Surface::createNoise() {
	obj::MapTerrain()->createNoise();
}

void Surface::updateGrid() {
	obj::MapGrid()->update();
}

void Surface::render(bool tracing) {

	if (GLB::MOUSE_LEFT && GLB::EDITOR) {
		float x1 = (GLB::MOUSE_X * GLB::WINDOW_WIDTH_ZOOMED / GLB::WINDOW_WIDTH + GAME::CAMERA_POS_X);
		float y1 = (GLB::MOUSE_Y * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT + GAME::CAMERA_POS_Y);
		obj::MapTerrain()->updateBuffers(x1, y1, "terrain", 2.f);
	}

	obj::MapTerrain()->render(tracing);

	if (GAME::GRID_IS_ACTIVE && !tracing) {
		obj::MapGrid()->render();
	}
}

Surface::~Surface()
{
}
