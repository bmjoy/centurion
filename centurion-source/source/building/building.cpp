#include "stb_image.h"  // manip. texture

#include "building.h"

Building::Building()
{
}

void Building::create() {
	std::ifstream path_scan(data["pass_path"].get<std::string>());
	json grid_data = json::parse(path_scan);
	building_grid = grid_data["grid"].get<std::vector<std::vector<int>>>();
	updatePassMatrix(building_grid, glm::vec2((int)position.x / PATH::CELL_GRID_SIZE - building_grid[0].size() / 2, (int)position.y / PATH::CELL_GRID_SIZE - building_grid.size() / 2));

	std::ifstream path_ent(data["ent_path"].get<std::string>());
	json ent_data = json::parse(path_ent);
	std::string texturePath = ent_data["path"].get<std::string>() + ent_data["sprites"][0]["name"].get<std::string>();
	unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	stbi_image_free(texture);

	clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
	textureID = obj::BSprite()->getTextureId(className);
}

void Building::render(bool picking, int clickID) {
	selected = (picking_id == clickID);
	obj::BSprite()->render(textureID, clickableInMinimap, position.x, position.y, w, h, picking, picking_id, selected, player->getPlayerColor());
}

Building::~Building()
{
}
