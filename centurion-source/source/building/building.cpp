#include "building.h"

Building::Building()
{
}

void Building::render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID) {
	selected = (picking_id == clickID);
	
	obj::BuildingSprite()->apply_projection_matrix(proj);
	obj::BuildingSprite()->apply_view_matrix(view);
	obj::BuildingSprite()->render(className, position.x, position.y, picking, picking_id, selected, &(*player).getPlayerColor());
}

void Building::create_pass() {
	std::ifstream path_scan(data["pass_path"].get<std::string>());
	grid_data = json::parse(path_scan);
	building_grid = grid_data["grid"].get<std::vector<std::vector<int>>>();
	updatePassMatrix(building_grid, glm::vec2((int)position.x/ PATH::CELL_GRID_SIZE - building_grid[0].size() / 2, (int)position.y/ PATH::CELL_GRID_SIZE - building_grid.size() / 2));
}

Building::~Building()
{
}
