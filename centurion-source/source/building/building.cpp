#include <object>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <game>
#include <math>

using namespace std;
using namespace glm;

namespace building {
	Building::Building(){
		type = "building";
		isCreated = false;
		waitingToBeErased = false;
		isPlaceable = true;
		buildingListSize = 0;
	}

	void Building::prepare() {
		/* file pass */
		string pass_path = data["pass_path"].get<string>();
		building_grid = astar::readPassMatrix(pass_path, className);

		ifstream path_ent(data["ent_path"].get<string>());
		if (!path_ent.good()) {
			//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
		}
		json ent_data = json::parse(path_ent);
		string texturePath = ent_data["path"].get<string>() + ent_data["sprites"][0]["name"].get<string>();
		unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
		stbi_image_free(texture);

		isCentralBuilding = (bool)data["is_central_building"].get<int>();
		clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
		//selectionSound = (sound)data["selectionSound"].get<string>(); TODO
		textureID = obj::BSprite()->getTextureId(className);
		isCreated = false;
	}

	bool Building::is_placeable() {
		bool placeable = astar::checkAvailability(building_grid, position);
		if (!isCentralBuilding) placeable = (placeable && is_near_to_central_building());
		return placeable;
	}

	bool Building::is_near_to_central_building() {
		bool ok = false;
		for (map<int, Building*>::iterator b = game::central_buildings.begin(); b != game::central_buildings.end(); b++) {
			int ID = b->first;
			float dist = math::euclidean_distance(b->second->get_position().x, b->second->get_position().y, position.x, position.y);
			if (dist < 1500.f) {
				set_settlement_building(game::central_buildings[ID]);
				set_settlement_name(game::central_buildings[ID]->get_settlement_name());
				ok = true;
				break;
			}
		}
		return ok;
	}

	void Building::create(string Name) {
		/* file pass */
		string pass_path = data["pass_path"].get<string>();
		if (building_grid.size() == 0) building_grid = astar::readPassMatrix(pass_path, className);
		update_pass();

		ifstream path_ent(data["ent_path"].get<string>());
		if (!path_ent.good()) {
			//showGameWarning("Error code 0x00000002\n\n  Unable to find (or communicate with) the audio device.\n  No sound will be played as long as the error persists.");
		}
		json ent_data = json::parse(path_ent);
		string texturePath = ent_data["path"].get<string>() + ent_data["sprites"][0]["name"].get<string>();
		unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
		stbi_image_free(texture);

		isCentralBuilding = (bool)data["is_central_building"].get<int>();
		clickableInMinimap = (bool)data["clickable_in_minimap"].get<int>();
		//selectionSound = (sound)data["selectionSound"].get<string>(); TODO
		textureID = obj::BSprite()->getTextureId(className);

		(Name == "") ? name = className + "_" + to_string(picking_id) : name = Name;
		isCreated = true;
	}

	void Building::update_pass() {
		astar::updatePassMatrix(building_grid, position);
	}
	void Building::clear_pass() {
		astar::clearPassMatrix(building_grid, position);
	}

	void Building::render(bool picking, int clickID, bool not_placeable) {

		not_placeable = (not_placeable || !isPlaceable);

		// keep updated not central buildings "settlement name"
		if (!isCentralBuilding && isCreated) 
			if (settl_name != central_building->get_settlement_name())
			    settl_name = central_building->get_settlement_name();

		// keep updated central buildings "subsidiaries buildings list"
		if (game::buildings.size() != buildingListSize && isCentralBuilding) {
			subs_buildings.clear();
			cout << "[DEBUG] Subsidiaries buildings to " + name + " have been updated. Their names are: \n";
			for (map<int, Building>::iterator bld = game::buildings.begin(); bld != game::buildings.end(); bld++) {
				int ID = bld->first;
				if (!bld->second.is_central_building()) {
					if (bld->second.get_settlement_name() == settl_name) {
						subs_buildings[ID] = &game::buildings[ID];
						cout << "   " << game::buildings[ID].get_name() << "\n";
					}
				}
			}
			buildingListSize = game::buildings.size();
		}

		// has the building been selected?
		selected = (picking_id == clickID);

		// rendering
		obj::BSprite()->render(textureID, clickableInMinimap, position.x, position.y, (float)w, (float)h, picking, picking_id, selected, player->getPlayerColor(), not_placeable);
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

	vector<int> Building::buildingsInSettlementIds(){
		vector<int> ids;
		for (map<int, Building*>::iterator sub = subs_buildings.begin(); sub != subs_buildings.end(); sub++) {
			ids.push_back(sub->first);
		}
		return ids;
	}

	Building::~Building(){}
}