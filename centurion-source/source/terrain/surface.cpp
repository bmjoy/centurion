#include "surface.h"

Surface::Surface()
{	
}

void Surface::create() {
	terrain = Terrain();
	int terrain_shd = terrain.compile();	
	terrain.create();
	//terrain.set_window(window);

	grid = Grid();
	SHD::GRID_SHADER_ID = grid.compile();

	grid.create();
}

void Surface::render(glm::mat4 projMat, glm::mat4 viewMat, bool tracing) {
	if (GLB::MOUSE_LEFT && !GAME::MINIMAP_IS_ACTIVE) {
		float x1 = (GLB::MOUSE_X + GAME::CAMERA_POS_X);
		float y1 = (GLB::MOUSE_Y + GAME::CAMERA_POS_Y);
		terrain.update_mesh(x1, y1);
	}

	terrain.render(projMat, viewMat, tracing);



	if (GAME::GRID_IS_ACTIVE && !tracing) {
		grid.render(projMat, viewMat);
	}
}

Surface::~Surface()
{
}
