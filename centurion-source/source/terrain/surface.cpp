#include "surface.h"

Surface::Surface()
{	
}

void Surface::create() {
	if (GLB::GAME) {
		grid = Grid();
		SHD::GRID_SHADER_ID = grid.compile();
		grid.create();
	}
}

void Surface::render(glm::mat4 projMat, glm::mat4 viewMat, bool tracing) {

	if (GLB::MOUSE_LEFT && GLB::EDITOR) {
		float x1 = (GLB::MOUSE_X * GLB::WINDOW_WIDTH_ZOOMED / GLB::WINDOW_WIDTH + GAME::CAMERA_POS_X);
		float y1 = (GLB::MOUSE_Y * GLB::WINDOW_HEIGHT_ZOOMED / GLB::WINDOW_HEIGHT + GAME::CAMERA_POS_Y);
		obj::MapTerrain()->updateBuffers(x1, y1, "terrain", 2.f);
	}

	obj::MapTerrain()->render(tracing);

	if (GAME::GRID_IS_ACTIVE && !tracing && GLB::GAME) {
		grid.render(projMat, viewMat);
	}
}

Surface::~Surface()
{
}
