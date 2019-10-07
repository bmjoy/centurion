#include "stb_image.h"  // manip. texture
#include "../unit/unit.h"
#include "building.h"

#include "../pathfinding/a_star.h"

Building::Building(){}

void Building::create() {
	std::ifstream path_scan(data["pass_path"].get<std::string>());
	json grid_data = json::parse(path_scan);
	building_grid = grid_data["grid"].get<std::vector<std::vector<int>>>();
	aStar::updatePassMatrix(building_grid, glm::vec2((int)position.x / aStar::cellGridSize - building_grid[0].size() / 2, (int)position.y / aStar::cellGridSize - building_grid.size() / 2));

	std::ifstream path_ent(data["ent_path"].get<std::string>());
	if (!path_ent.good()) {
		//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
	}
	json ent_data = json::parse(path_ent);
	std::string texturePath = ent_data["path"].get<std::string>() + ent_data["sprites"][0]["name"].get<std::string>();
	unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	stbi_image_free(texture);

	clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
	//selectionSound = (sound)data["selectionSound"].get<std::string>(); TODO
	textureID = obj::BSprite()->getTextureId(className);
}

void Building::render(bool picking, int clickID) {
	selected = (picking_id == clickID);
	obj::BSprite()->render(textureID, clickableInMinimap, position.x, position.y, (float)w, (float)h, picking, picking_id, selected, player->getPlayerColor());
}

int Building::UnitsInBuilding(){
	if (clickableInMinimap) {
		return (int)unitsInside.size();
	}
	else {
		return 0;
	}
}

std::vector<Unit> Building::UnitsInHolder() {
	if (clickableInMinimap) {
		return unitsInside;
	}
	else {
		return {};
	}
}

Building::~Building()
{
}