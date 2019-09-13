#include "building.h"

Building::Building()
{
}

void Building::create() {
	std::ifstream path(data["ent_path"].get<std::string>());
	building_sprite = BSprite(SHD::BSPRITE_SHADER_ID);
	building_sprite.create(json::parse(path), player_color);
}

void Building::render(glm::mat4 proj, glm::mat4 view, bool picking) {
	building_sprite.apply_projection_matrix(proj);
	building_sprite.apply_view_matrix(view);
	building_sprite.render(position.x, position.y, picking, picking_id);
}

void Building::create_pass() {
	std::ifstream path_scan(data["pass_path"].get<std::string>());
	grid_data = json::parse(path_scan);

	building_grid = grid_data["grid"].get<std::vector<std::vector<int>>>();

	updatePassMatrix(building_grid, glm::vec2((int)position.x/ PATH::CELL_GRID_SIZE - building_grid[0].size() / 2, (int)position.y/ PATH::CELL_GRID_SIZE - building_grid.size() / 2));
}

void Building::select(bool b) {
	building_sprite.IsSelected = b;
}

Building::~Building()
{
}
