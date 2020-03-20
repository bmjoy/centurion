#include "building.h"

#include <engine.h>
#include <stb_image.h>
#include <player/player.h>
#include <pathfinding/pathfinding.h>
#include <game/strategy.h>
#include <maths.h>
#include <interface>
#include <building_sprite.h>
#include <logger.h>
#include <picking.h>

using namespace std;
using namespace glm;

Building::Building() 
{
	this->SetType("building");
	this->bIsCreated = false;
	this->waitingToBeErased = false;
	this->bIsPlaceable = true;
	this->buildingListSize = 0;
	this->bIsTownhall = false;
	this->bIsVillageHall = false;
	this->bIsOutpost = false;
	this->bIsShipyard = false;
}

Settlement Building::GetSettlement(void)
{
	return this->settlement;
}
void Building::SetSettlement(const Settlement par_settlement)
{
	this->settlement = par_settlement;
}

bool Building::IsClickableInMimimap(void)
{
	return this->bIsClickableInMimimap;
}

void Building::CanBeClickableInMimimap(const bool par_clickable)
{
	this->bIsClickableInMimimap = par_clickable;
}

bool Building::IsPlaceable(void)
{
	vec3 var_position = this->GetPosition();
	bool placeable = astar::checkAvailability(pass_grid, var_position);
	//string indCategory = "";
	//bool nearToIndependent = is_near_to_independent(&indCategory);
	//if (!this->settlement.IsIndipendent())
	//{
	//	//placeable = placeable && nearToIndependent && indCategory == this->category;
	//	placeable = placeable && indCategory == this->category;
	//}
	return placeable;
}

void Building::SetPlaceable(bool placeable)
{
	this->bIsPlaceable = placeable;
}

bool Building::IsCentralBuilding(void)
{
	return this->bIsCentralBuilding;
}

string Building::GetCategory(void)
{
	return this->category;
}

bool Building::IsVillageHall(void)
{
	return this->bIsVillageHall;
}

bool Building::IsTownhall(void)
{
	return this->bIsTownhall;
}

bool Building::IsOutpost(void)
{
	return this->bIsOutpost;
}

bool Building::IsShipyard(void)
{
	return this->bIsShipyard;
}

void Building::StartGoldProduction(void)
{
	this->bCanProduceGold = true;
}

void Building::StopGoldProduction(void)
{
	this->bCanProduceGold = true;
}

void Building::StartFoodProduction(void)
{
	this->bCanProduceFood = true;
}

void Building::StopFoodProduction(void)
{
	this->bCanProduceFood = false;
}

void Building::prepare() 
{
	//this->SetType(data["type"].get<string>());
	//this->SetRaceName(data["race"].get<string>());
	//this->SetClassName(data["class_name"].get<string>());
	//this->category = data["category"].get<string>();
	//this->ent_path = data["ent_path"].get<string>();
	//this->pass_path = data["pass_path"].get<string>();
	//this->bIsClickableInMimimap = (bool)data["clickable_in_minimap"].get<int>();
	//this->bIsTownhall = (this->GetClassName().substr(1) == "townhall");
	//this->bIsVillageHall = (this->GetClassName().substr(1) == "village");

	///* file pass */
	//string str_className = this->GetClassName();
	//this->pass_grid = astar::readPassMatrix(this->pass_path, str_className);

	//vec2 spriteSize = getSpriteSize(this->ent_path);
	//this->sprite_width = spriteSize.x;
	//this->sprite_height = spriteSize.y;
	//this->textureID = BSprite()->getTextureId(this->GetClassName());

	//this->bIsCreated = false;
}

//void Building::Create(string className) {

	//this->SetType(data["type"].get<string>());
	//this->SetRaceName(data["race"].get<string>());
	//this->SetClassName(data["class_name"].get<string>());
	////prop.is_indipendent = (bool)data["is_independent"].get<int>();
	////this->category = data["category"].get<string>();
	//this->ent_path = data["ent_path"].get<string>();
	//this->pass_path = data["pass_path"].get<string>();
	//this->bIsClickableInMimimap = (bool)data["clickable_in_minimap"].get<int>();
	//this->bIsTownhall = (this->GetClassName().substr(1) == "townhall");
	//this->bIsVillageHall = (this->GetClassName().substr(1) == "village");

	///* file pass */
	//string str_className = this->GetClassName();
	//if (pass_grid.size() == 0) pass_grid = astar::readPassMatrix(this->pass_path, str_className);
	//update_pass();

	//vec2 spriteSize = getSpriteSize(this->ent_path);
	//this->sprite_width = spriteSize.x;
	//this->sprite_height = spriteSize.y;
	//this->textureID = BSprite()->getTextureId(this->GetClassName());
	//string name;
	//(Name == "") ? name = this->GetClassName() + "_" + to_string(this->GetPickingID()) : name = Name;
	//this->SetName(name);

	//buildingUI = new game::ObjectUI();
	//buildingUI->create(this->GetClassName());

	//// selection circle (editor only)
	//circle[0] = gui::Circle();
	//circle[0].create("border", 0.f, 0.f, data["radius"].get<float>() * 2.f, data["radius"].get<float>() * 2.f * 2.25f / 3.f, 8.f, "center");

	//// townhall radius (editor only)
	//circle[1] = gui::Circle();
	//circle[1].create("border", 0.f, 0.f, TOWNHALL_RADIUS * 2.f, TOWNHALL_RADIUS * 2.f, 10.f, "center");
	//this->bIsCreated = true;

//}

void Building::SetBuildingProperties(ObjectData::ObjectXMLClassData &objData)
{
	// qui si puo fare cosi oppure si puo settare radius come protetta (e non privata)
	// e quindi passare &radius al posto di creare e passare &_radius
	// si risparmia giusto una riga di codice :)
	float _radius = 0.f;
	ObjectData::TryParseFloat(objData.GetPropertiesMap(), "radius", &_radius);
	this->SetRadius(_radius);

	// TryParseFloat, TryParseInteger, TryParseString
}

void Building::render(bool picking, int clickID, bool not_placeable) {

	BSprite()->render(spriteData, GetPosition().x, GetPosition().y, picking, false, vec3(0), false);

	//---------------------------

	//not_placeable = (not_placeable || !this->bIsPlaceable);

	// keep updated not central buildings "settlement name"
	//if (!this->settlement.IsIndipendent() && isCreated)
		//if (this->settlement.GetSettlementName() != independent->GetSettlement().GetSettlementName())
		//{
			//string strSetName;
			//strSetName = independent->GetSettlement().GetSettlementName();
			//this->GetSettlement().SetSettlementName(strSetName);
		//}
		
	// keep updated central buildings "subsidiaries buildings list"
	//if (Game::GetNumberOfBuildings() != buildingListSize && (this->bIsTownhall || this->bIsVillageHall)) 
	//{
	//	subs_buildings.clear();
	//	int k = 0;

	//	vector<Building*> listOfBuildings = Game::GetListOfBuildings();
	//	for (int i = 0; i < listOfBuildings.size(); i++) 
	//	{
	//		Building* bld = listOfBuildings[i];
	//		if (bld->GetSettlement().IsIndipendent() == false) 
	//		{
	//			if (bld->GetSettlement().GetSettlementName() == this->GetSettlement().GetSettlementName())
	//			{
	//				if (k == 0)
	//				{ 
	//					cout << "[DEBUG] Subsidiaries buildings to " + this->GetName() + " have been updated. Their names are: \n"; 
	//				}
	//				subs_buildings[bld->GetPickingID()] = bld;
	//				cout << "   " << bld->GetName() << "\n";
	//				k++;
	//			}
	//		}
	//	}
	//	buildingListSize = Game::GetNumberOfBuildings();
	//}

	//// has the building been selected?
	//bool bSelected = (this->GetPickingID() == clickID);
	//this->Select(bSelected);
	//if (bSelected) {
	//	if (Game::GetSelectedObject() != this) {
	//		Game::SetSelectedObject(this);
	//	}
	//}
	//if (Engine::getEnvironment() == EDITOR_ENV && !Game::Minimap::IsActive()) {
	//	if (this->IsSelected() && !editor::addingObject) circle[0].render(vec4(255.f), this->GetPosition().x, this->GetPosition().y - data["radius"].get<float>() / 15.5f); // selection circle (editor only)
	//	if (this->IsSelected() && (this->bIsTownhall || this->bIsVillageHall) && !editor::addingObject) circle[1].render(vec4(0, 255, 255, 255), this->GetPosition().x, this->GetPosition().y); // selection circle (editor only)
	//}

	//if (Engine::getEnvironment() == STRATEGY_ENV && this->IsSelected()) {
	//	game::GAME_UI()->set_ui(buildingUI);
	//}

	//// rendering
	//BSprite()->render(this->textureID, this->bIsClickableInMimimap, this->GetPosition().x, this->GetPosition().y, this->sprite_width, this->sprite_height, picking, this->GetPickingID(), this->IsSelected(), player->getPlayerColor(), not_placeable);
}

void Building::SetStatus(const bool bIsCreated)
{
	this->bIsCreated = bIsCreated;
}

/*
vector<int> Building::buildingsInSettlementIds() 
{
	vector<int> ids;
	for (map<int, Building*>::iterator sub = subs_buildings.begin(); sub != subs_buildings.end(); sub++) 
	{
		ids.push_back(sub->first);
	}
	return ids;
}
*/

Building::~Building() {}

void Building::IAmACentralBuilding(void)
{
	this->settlement = Settlement();
	this->bIsCentralBuilding = true;
}

void Building::IAmATownhall(void)
{
	this->bIsTownhall = true;
}

void Building::IAmAnOutpost(void)
{
	this->bIsOutpost = true;
}

void Building::IAmAVillageHall(void)
{
	this->bIsVillageHall = true;
}

void Building::IAmAShipyard(void)
{
	this->bIsShipyard = true;
}
