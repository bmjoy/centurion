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
	terrain.render(projMat, viewMat, tracing);

	if (GAME::GRID_IS_ACTIVE) {
		grid.render(projMat, viewMat);
	}
}

Surface::~Surface()
{
}
