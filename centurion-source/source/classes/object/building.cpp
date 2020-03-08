#include <object/building.h>
#include <engine/engine.h>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <game/strategy.h>
#include <math>
#include <interface>

using namespace std;
using namespace glm;

Building::Building() {
	type = "building";
	isCreated = false;
	waitingToBeErased = false;
	isPlaceable = true;
	buildingListSize = 0;
	prop.is_townhall = false;
	prop.is_villagehall = false;
}

void Building::prepare() {

	prop.type = data["type"].get<string>();
	prop.race = data["race"].get<string>();
	prop.class_name = data["class_name"].get<string>();
	prop.is_indipendent = (bool)data["is_independent"].get<int>();
	prop.category = data["category"].get<string>();
	prop.ent_path = data["ent_path"].get<string>();
	prop.pass_path = data["pass_path"].get<string>();
	prop.clickable_in_minimap = (bool)data["clickable_in_minimap"].get<int>();
	prop.is_townhall = (prop.class_name.substr(1) == "townhall");
	prop.is_villagehall = (prop.class_name.substr(1) == "village");

	/* file pass */
	pass_grid = astar::readPassMatrix(prop.pass_path, className);

	vec2 spriteSize = getSpriteSize(prop.ent_path);
	prop.sprite_width = spriteSize.x;
	prop.sprite_height = spriteSize.y;
	prop.textureID = obj::BSprite()->getTextureId(className);

	isCreated = false;
}

bool Building::is_placeable() {
	bool placeable = astar::checkAvailability(pass_grid, position);
	string indCategory = "";
	bool nearToIndependent = is_near_to_independent(&indCategory);
	if (!prop.is_indipendent) placeable = placeable && nearToIndependent && indCategory == prop.category;
	return placeable;
}

bool Building::is_near_to_independent(string *Category) {
	bool ok = false;
	(*Category) = "";
	for (map<int, Building*>::iterator b = independent_buildings.begin(); b != independent_buildings.end(); b++) {
		int ID = b->first;
		float dist = math::euclidean_distance(b->second->get_position().x, b->second->get_position().y, position.x, position.y);
		if (dist < 1500.f) {
			set_settlement_building(independent_buildings[ID]);
			set_settlement_name(independent_buildings[ID]->get_settlement_name());
			(*Category) = independent_buildings[ID]->getCategory();
			ok = true;
			break;
		}
	}
	return ok;
}

void Building::create(string Name) {
	prop.type = data["type"].get<string>();
	prop.race = data["race"].get<string>();
	prop.class_name = data["class_name"].get<string>();
	prop.is_indipendent = (bool)data["is_independent"].get<int>();
	prop.category = data["category"].get<string>();
	prop.ent_path = data["ent_path"].get<string>();
	prop.pass_path = data["pass_path"].get<string>();
	prop.clickable_in_minimap = (bool)data["clickable_in_minimap"].get<int>();
	prop.is_townhall = (prop.class_name.substr(1) == "townhall");
	prop.is_villagehall = (prop.class_name.substr(1) == "village");

	/* file pass */
	if (pass_grid.size() == 0) pass_grid = astar::readPassMatrix(prop.pass_path, className);
	update_pass();

	vec2 spriteSize = getSpriteSize(prop.ent_path);
	prop.sprite_width = spriteSize.x;
	prop.sprite_height = spriteSize.y;
	prop.textureID = obj::BSprite()->getTextureId(className);

	(Name == "") ? name = className + "_" + to_string(picking_id) : name = Name;
	buildingUI = new game::ObjectUI();
	buildingUI->create(prop.class_name);

	// selection circle (editor only)
	circle[0] = gui::Circle();
	circle[0].create("border", 0.f, 0.f, data["radius"].get<float>() * 2.f, data["radius"].get<float>() * 2.f * 2.25f / 3.f, 8.f, "center");

	// townhall radius (editor only)
	circle[1] = gui::Circle();
	circle[1].create("border", 0.f, 0.f, TOWNHALL_RADIUS * 2.f, TOWNHALL_RADIUS * 2.f, 10.f, "center");
	isCreated = true;
}



void Building::render(bool picking, int clickID, bool not_placeable) {
	not_placeable = (not_placeable || !isPlaceable);

	// keep updated not central buildings "settlement name"
	/*if (!prop.is_indipendent && isCreated)
		if (settl_name != independent->get_settlement_name())
			settl_name = independent->get_settlement_name();*/

	// keep updated central buildings "subsidiaries buildings list"
	
	/*if (buildings.size() != buildingListSize && (prop.is_townhall || prop.is_villagehall)) {
		subs_buildings.clear();
		int k = 0;
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			int ID = bld->first;
			if (!bld->second.is_independent()) {
				if (bld->second.get_settlement_name() == settl_name) {
					if (k == 0) { cout << "[DEBUG] Subsidiaries buildings to " + name + " have been updated. Their names are: \n"; }
					subs_buildings[ID] = &buildings[ID];
					cout << "   " << buildings[ID].get_name() << "\n";
					k++;
				}
			}
		}
		buildingListSize = buildings.size();
	}*/

	// has the building been selected?
	selected = (picking_id == clickID);

	if (engine::Engine::getEnvironment() == "editor" && !Game::Minimap::IsActive()) {
		if (selected && !editor::addingObject) circle[0].render(vec4(255.f), position.x, position.y - data["radius"].get<float>() / 15.5f); // selection circle (editor only)
		if (selected && (prop.is_townhall || prop.is_villagehall) && !editor::addingObject) circle[1].render(vec4(0, 255, 255, 255), position.x, position.y); // selection circle (editor only)
	}

	if (engine::Engine::getEnvironment() == "game" && selected) {
		game::GAME_UI()->set_ui(buildingUI);
	}

	// rendering
	obj::BSprite()->render(prop.textureID, prop.clickable_in_minimap, position.x, position.y, prop.sprite_width, prop.sprite_height, picking, picking_id, selected, player->getPlayerColor(), not_placeable);
}

int Building::UnitsInBuilding() {
	if (prop.clickable_in_minimap) {
		return (int)unitsInside.size();
	}
	else {
		return 0;
	}
}

vector<Unit> Building::UnitsInHolder() {
	if (prop.clickable_in_minimap) {
		return unitsInside;
	}
	else {
		return {};
	}
}

vector<int> Building::buildingsInSettlementIds() {
	vector<int> ids;
	for (map<int, Building*>::iterator sub = subs_buildings.begin(); sub != subs_buildings.end(); sub++) {
		ids.push_back(sub->first);
	}
	return ids;
}

vec2 Building::getSpriteSize(string ent_path) {
	ifstream path_ent(ent_path);
	if (!path_ent.good()) {
		stringstream ss;

		ss << "Unable to find or process " << ent_path << ". The relative building, therefore, won't be loaded into the game.";
		Logger::Warn(ss.str());
	}
	json ent_data = json::parse(path_ent);
	string texturePath = ent_data["path"].get<string>() + ent_data["sprites"][0]["name"].get<string>();
	int w, h, nrChannels;
	unsigned char *texture = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	stbi_image_free(texture);

	return vec2(w, h);
}

Building::~Building() {}
