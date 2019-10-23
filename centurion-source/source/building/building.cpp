#include <object>
#include <stb_image.h>
#include <player>
#include <pathfinding>

using namespace std;
using namespace glm;

namespace building {
	Building::Building(){
		type = "building";
	}

	void Building::prepare() {
		
		ifstream path_ent(data["ent_path"].get<string>());
		if (!path_ent.good()) {
			//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
		}
		json ent_data = json::parse(path_ent);
		string texturePath = ent_data["path"].get<string>() + ent_data["sprites"][0]["name"].get<string>();
		unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
		stbi_image_free(texture);

		clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
		//selectionSound = (sound)data["selectionSound"].get<string>(); TODO
		textureID = obj::BSprite()->getTextureId(className);
	}
	void Building::create(string Name) {
		ifstream path_scan(data["pass_path"].get<string>());
		json grid_data = json::parse(path_scan);
		building_grid = grid_data["grid"].get<vector<vector<int>>>();
		astar::updatePassMatrix(building_grid, vec2((int)position.x / astar::cellGridSize - building_grid[0].size() / 2, (int)position.y / astar::cellGridSize - building_grid.size() / 2));

		ifstream path_ent(data["ent_path"].get<string>());
		if (!path_ent.good()) {
			//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
		}
		json ent_data = json::parse(path_ent);
		string texturePath = ent_data["path"].get<string>() + ent_data["sprites"][0]["name"].get<string>();
		unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
		stbi_image_free(texture);

		clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
		//selectionSound = (sound)data["selectionSound"].get<string>(); TODO
		textureID = obj::BSprite()->getTextureId(className);

		(Name == "") ? name = className + "_" + to_string(picking_id) : name = Name;
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

	vector<unit::Unit> Building::UnitsInHolder() {
		if (clickableInMinimap) {
			return unitsInside;
		}
		else {
			return {};
		}
	}

	Building::~Building(){}
}