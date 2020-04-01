#include "building.h"
#include "Unit.h"
#include <pathfinding/pathfinding.h>
#include <building_sprite.h>
#include <logger.h>
#include <math.h>

using namespace std;
using namespace glm;

Building::Building() 
{
	this->bIsCreated = false;
	this->waitingToBeErased = false;
	this->bIsPlaceable = true;
	this->bIsTownhall = false;
	this->bIsVillageHall = false;
	this->bIsOutpost = false;
	this->bIsShipyard = false;
	this->settlement = nullptr;
}

Settlement *Building::GetSettlement(void)
{
	return this->settlement;
}
void Building::SetSettlement(Settlement *par_settlement)
{
	this->settlement = par_settlement;
}

void Building::SetCategory(const string par_category)
{
	this->category = par_category;
}

string Building::GetCategory(void)
{
	return this->category;
}

void Building::SetMaxHealth(const unsigned int par_maxHealth)
{
	this->maxHealth = par_maxHealth;
}

unsigned int Building::GetMaxHealth(void)
{
	return this->maxHealth;
}

void Building::SetRepairRate(const unsigned int par_repairRate)
{
	this->repairRate = par_repairRate;
}

unsigned int Building::GetRepairRate(void)
{
	return this->repairRate;
}

void Building::SetLoyaltyFearHealthPercent(const unsigned int par_loyaltyFearHealthPercent)
{
	this->loyaltyFearHealthPercent = par_loyaltyFearHealthPercent;
}

unsigned int Building::GetLoyaltyFearHealthPercent(void)
{
	return this->loyaltyFearHealthPercent;
}

void Building::CanBeClickableInMimimap(const bool par_clickable)
{
	this->bIsClickableInMimimap = par_clickable;
}

bool Building::IsClickableInMimimap(void)
{
	return this->bIsClickableInMimimap;
}

bool Building::IsCentralBuilding(void)
{
	return this->bIsCentralBuilding;
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

void Building::SetPlaceable(bool placeable)
{
	this->bIsPlaceable = placeable;
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

void Building::SetEntPath(const string par_ent_path)
{
	this->ent_path = par_ent_path;
}

void Building::SetPassPath(const string par_pass_path)
{
	this->pass_path = par_pass_path;
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

/*
//void Building::Create(string className) 
//{
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

	// file pass 
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
*/

bool Building::SetBuildingProperties(ObjectData::ObjectXMLClassData &objData)
{
	bool bBuildingCreated = false;
	// TryParseFloat, TryParseInteger, TryParseString
	float fProperty = 0.f;
	int iProperty = 0;
	string strProperty = "";

	//Building's properties:
	ObjectData::TryParseString(objData.GetPropertiesMap(), "isCentralBuilding", &strProperty);
	this->bIsCentralBuilding = strProperty == "true" ? true : false;
	bBuildingCreated = this->FindASettlement(this);
	if (bBuildingCreated == true)
	{
		ObjectData::TryParseString(objData.GetPropertiesMap(), "category", &strProperty);
		this->category = strProperty;
		ObjectData::TryParseInteger(objData.GetPropertiesMap(), "maxHealth", &iProperty);
		this->maxHealth = iProperty;
		ObjectData::TryParseInteger(objData.GetPropertiesMap(), "repairRate", &iProperty);
		this->repairRate = iProperty;
		ObjectData::TryParseInteger(objData.GetPropertiesMap(), "loyaltyFearHealthPercent", &iProperty);
		this->loyaltyFearHealthPercent = iProperty;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "clickable_in_minimap", &strProperty);
		this->bIsClickableInMimimap = strProperty == "true" ? true : false;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "autoRepair", &strProperty);
		this->bAutoRepair = strProperty == "true" ? true : false;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "canProduceGold", &strProperty);
		this->bCanProduceGold = strProperty == "true" ? true : false;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "canProduceFood", &strProperty);
		this->bCanProduceFood = strProperty == "true" ? true : false;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "ent_path", &strProperty);
		this->ent_path = strProperty;
		ObjectData::TryParseString(objData.GetPropertiesMap(), "pass_path", &strProperty);
		this->pass_path = strProperty;
	}
	return bBuildingCreated;
}

void Building::Render(const bool picking, const unsigned int clickID, const bool not_placeable)
{
	const bool bCurrentlySelected = this->CheckIfSelected(clickID);

	BSprite()->Render(spriteData, GetPosition().x, GetPosition().y, picking, bCurrentlySelected, vec3(0), false);

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
			//Game::SetSelectedObject(this);
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

Building::~Building() 
{
	if (this->settlement != nullptr)
	{
		delete this->settlement;
		this->settlement = nullptr;
	}
}

#pragma region Private members
vector<Building::SettlementSet> Building::settlementsList;
bool Building::FindASettlement(Building* b)
{
	bool bSettlementDiscovered = false;
	size_t numOfSettlements = Building::settlementsList.size();
	float b_xPos = b->get_xPos();
	float b_yPos = b->get_yPos();

	if (b->bIsCentralBuilding == false)
	{
		for (unsigned int settlementsCounter = 0; settlementsCounter < numOfSettlements && bSettlementDiscovered == false; settlementsCounter++)
		{
			if ( (b->GetPlayer() == Building::settlementsList[settlementsCounter].set->GetPlayer())
				&& (Building::settlementsList[settlementsCounter].set->IsIndipendent() == false) )
			{
				size_t numOfBuildings = Building::settlementsList[settlementsCounter].xPoint.size();
				for (unsigned int buildingsCounter = 0; buildingsCounter < numOfBuildings && bSettlementDiscovered == false; buildingsCounter++)
				{
					float xPos = Building::settlementsList[settlementsCounter].xPoint[buildingsCounter];
					float yPos = Building::settlementsList[settlementsCounter].xPoint[buildingsCounter];
					float distance = sqrt(pow(b_xPos - xPos, 2) + pow(b_yPos - yPos, 2));
					if (distance <= b->GetRadius()) //If the two buildings are close enough 
					{
						float distance2 = sqrt(pow((b_xPos - RADIUS_OFFSET) - (xPos - RADIUS_OFFSET), 2) + pow((b_yPos - RADIUS_OFFSET) - (yPos -RADIUS_OFFSET), 2));
						if (distance2 > b->GetRadius() + Building::settlementsList[settlementsCounter].radius[buildingsCounter]) //If the two buildings don't intersect each others 
						{
							b->settlement = Building::settlementsList[settlementsCounter].set;
							Building::settlementsList[settlementsCounter].xPoint.push_back(b_xPos);
							Building::settlementsList[settlementsCounter].yPoint.push_back(b_yPos);
							Building::settlementsList[settlementsCounter].radius.push_back(b->GetRadius());
							bSettlementDiscovered = true;
							bSettlementDiscovered = bSettlementDiscovered = b->settlement->AddBuildingToSettlement(b); //Here, it should be return always true.
						}
					}
				}
			}
		}
	}
	else
	{
		b->settlement = new Settlement(b->GetPlayer());
		Building::settlementsList[numOfSettlements].set = b->settlement;
		Building::settlementsList[numOfSettlements].xPoint.push_back(b_xPos);
		Building::settlementsList[numOfSettlements].yPoint.push_back(b_yPos);
		Building::settlementsList[numOfSettlements].radius.push_back(b->GetRadius());
		bSettlementDiscovered = b->settlement->AddBuildingToSettlement(b); //Here, it should be return always true.
	}
	return bSettlementDiscovered;
}
#pragma endregion