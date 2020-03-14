#include <object/building.h>
#include <engine.h>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <game/strategy.h>
#include <maths.h>
#include <interface>

#include <building_sprite.h>

using namespace std;
using namespace glm;

Building::Building() 
{
	this->SetType("building");
	isCreated = false;
	waitingToBeErased = false;
	isPlaceable = true;
	buildingListSize = 0;
	prop.is_townhall = false;
	prop.is_villagehall = false;
}

Settlement Building::GetSettlement(void)
{
	return this->settlement;
}
void Building::SetSettlement(const Settlement par_settlement)
{
	this->settlement = par_settlement;
}

void Building::prepare() 
{
	this->SetType(data["type"].get<string>());
	this->SetRaceName(data["race"].get<string>());
	this->SetClassName(data["class_name"].get<string>());
	prop.is_indipendent = (bool)data["is_independent"].get<int>();
	prop.category = data["category"].get<string>();
	prop.ent_path = data["ent_path"].get<string>();
	prop.pass_path = data["pass_path"].get<string>();
	prop.clickable_in_minimap = (bool)data["clickable_in_minimap"].get<int>();
	prop.is_townhall = (this->GetClassName().substr(1) == "townhall");
	prop.is_villagehall = (this->GetClassName().substr(1) == "village");

	/* file pass */
	string str_className = this->GetClassName();
	pass_grid = astar::readPassMatrix(prop.pass_path, str_className);

	vec2 spriteSize = getSpriteSize(prop.ent_path);
	prop.sprite_width = spriteSize.x;
	prop.sprite_height = spriteSize.y;
	prop.textureID = BSprite()->getTextureId(this->GetClassName());

	isCreated = false;
}

bool Building::is_placeable() {
	vec3 var_position = this->GetPosition();
	bool placeable = astar::checkAvailability(pass_grid, var_position);
	string indCategory = "";
	bool nearToIndependent = is_near_to_independent(&indCategory);
	if (!prop.is_indipendent) placeable = placeable && nearToIndependent && indCategory == prop.category;
	return placeable;
}

bool Building::is_near_to_independent(string *Category) {
	bool ok = false;
	(*Category) = "";
	vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();
	for (int i = 0; i < listOfIndipBuildings.size(); i++) {
		Building* bld = listOfIndipBuildings[i];
		float dist = math::euclidean_distance(bld->GetPosition().x, bld->GetPosition().y, this->GetPosition().x, this->GetPosition().y);
		if (dist < 1500.f) {
			set_settlement_building(bld);
			string strSetName = bld->GetSettlement().GetSettlementName();
			this->GetSettlement().SetSettlementName(strSetName);
			(*Category) = bld->getCategory();
			ok = true;
			break;
		}
	}
	return ok;
}

void Building::create(string Name) {
	this->SetType(data["type"].get<string>());
	this->SetRaceName(data["race"].get<string>());
	this->SetClassName(data["class_name"].get<string>());
	prop.is_indipendent = (bool)data["is_independent"].get<int>();
	prop.category = data["category"].get<string>();
	prop.ent_path = data["ent_path"].get<string>();
	prop.pass_path = data["pass_path"].get<string>();
	prop.clickable_in_minimap = (bool)data["clickable_in_minimap"].get<int>();
	prop.is_townhall = (this->GetClassName().substr(1) == "townhall");
	prop.is_villagehall = (this->GetClassName().substr(1) == "village");

	/* file pass */
	string str_className = this->GetClassName();
	if (pass_grid.size() == 0) pass_grid = astar::readPassMatrix(prop.pass_path, str_className);
	update_pass();

	vec2 spriteSize = getSpriteSize(prop.ent_path);
	prop.sprite_width = spriteSize.x;
	prop.sprite_height = spriteSize.y;
	prop.textureID = BSprite()->getTextureId(this->GetClassName());
	string name;
	(Name == "") ? name = this->GetClassName() + "_" + to_string(this->GetPickingID()) : name = Name;
	this->SetName(name);

	buildingUI = new game::ObjectUI();
	buildingUI->create(this->GetClassName());

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
	if (!prop.is_indipendent && isCreated)
		if (this->GetSettlement().GetSettlementName() != independent->GetSettlement().GetSettlementName())
		{
			string strSetName;
			strSetName = independent->GetSettlement().GetSettlementName();
			this->GetSettlement().SetSettlementName(strSetName);
		}
		
	// keep updated central buildings "subsidiaries buildings list"
	if (Game::GetNumberOfBuildings() != buildingListSize && (prop.is_townhall || prop.is_villagehall)) {
		subs_buildings.clear();
		int k = 0;

		vector<Building*> listOfBuildings = Game::GetListOfBuildings();
		for (int i = 0; i < listOfBuildings.size(); i++) {
			Building* bld = listOfBuildings[i];
			if (bld->is_independent() == false) {
				if (bld->GetSettlement().GetSettlementName() == this->GetSettlement().GetSettlementName())
				{
					if (k == 0) { cout << "[DEBUG] Subsidiaries buildings to " + this->GetName() + " have been updated. Their names are: \n"; }
					subs_buildings[bld->GetPickingID()] = bld;
					cout << "   " << bld->GetName() << "\n";
					k++;
				}
			}
		}
		buildingListSize = Game::GetNumberOfBuildings();
	}

	// has the building been selected?
	bool bSelected = (this->GetPickingID() == clickID);
	this->Select(bSelected);

	if (Engine::getEnvironment() == "editor" && !Game::Minimap::IsActive()) {
		if (this->IsSelected() && !editor::addingObject) circle[0].render(vec4(255.f), this->GetPosition().x, this->GetPosition().y - data["radius"].get<float>() / 15.5f); // selection circle (editor only)
		if (this->IsSelected() && (prop.is_townhall || prop.is_villagehall) && !editor::addingObject) circle[1].render(vec4(0, 255, 255, 255), this->GetPosition().x, this->GetPosition().y); // selection circle (editor only)
	}

	if (Engine::getEnvironment() == "game" && this->IsSelected()) {
		game::GAME_UI()->set_ui(buildingUI);
	}

	// rendering
	BSprite()->render(prop.textureID, prop.clickable_in_minimap, this->GetPosition().x, this->GetPosition().y, prop.sprite_width, prop.sprite_height, picking, this->GetPickingID(), this->IsSelected(), player->getPlayerColor(), not_placeable);
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