#include "building.h"
#include "Unit.h"
#include <pathfinding/pathfinding.h>
#include <building_sprite.h>
#include <logger.h>
#include <engine.h>

using namespace std;
using namespace glm;

Building::Building(void) 
{
	this->bIsBeingMoved = false;
	this->MarkAsSelected(false);
	this->bIsCreated = false;
	this->waitingToBeErased = false;
	this->bIsPlaceable = true;
	this->bIsTownhall = false;
	this->bIsVillageHall = false;
	this->bIsOutpost = false;
	this->bIsShipyard = false;
	this->settlement = nullptr;
	this->SetType("cpp_buildingclass");
	this->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 10.f));
}

Settlement *Building::GetSettlement(void)
{
	return this->settlement;
}
void Building::SetSettlement(Settlement *par_settlement)
{
	this->settlement = par_settlement;
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

void Building::CheckIfPlaceable(void)
{
	this->bIsPlaceable = true;
	vec3 var_position = this->GetPosition();
	std::vector<std::vector<unsigned int>> passGrid = this->GetPass();
	this->bIsPlaceable = astar::CheckObjectPassAvailability(passGrid, var_position);
	//string indCategory = "";
	//bool nearToIndependent = is_near_to_independent(&indCategory);
	//if (!this->settlement.IsIndipendent())
	//{
	//	//placeable = placeable && nearToIndependent && indCategory == this->category;
	//	placeable = placeable && indCategory == this->category;
	//}
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

bool Building::SetBuildingProperties(ObjectData::ObjectXMLClassData &objData, const bool _temporary)
{
	bool bBuildingCreated = false;
	// TryParseFloat, TryParseInteger, TryParseString
	float fProperty = 0.f;
	int iProperty = 0;
	string strProperty = "";

	//Building's properties:
	ObjectData::TryParseString(objData.GetPropertiesMap(), "isCentralBuilding", &strProperty);
	this->bIsCentralBuilding = strProperty == "true" ? true : false;
	ObjectData::TryParseString(objData.GetPropertiesMap(), "pass_path", &strProperty);
	this->pass_path = strProperty;
	this->SetPass(strProperty);
	
	bBuildingCreated = this->FindASettlement(this, _temporary);
	if (_temporary == true)
		return bBuildingCreated;
	this->UpdatePass();
	if (bBuildingCreated == true)
	{
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
	}
	return bBuildingCreated;
}

void Building::Render(const bool picking, const unsigned int clickID)
{
	const bool bCurrentlySelected = this->CheckIfSelected(clickID);

	this->bIsPlaceable = true;
	if (this->bIsBeingMoved == true)
	{
		this->CheckIfPlaceable();
	}

	BSprite()->Render(spriteData, GetPosition().x, GetPosition().y, picking, bCurrentlySelected, vec3(0), this->bIsPlaceable);
}

void Building::SetStatus(const bool bIsCreated)
{
	this->bIsCreated = bIsCreated;
}

Building::~Building(void) 
{
	if (this->settlement != nullptr)
	{
		delete this->settlement;
		this->settlement = nullptr;
	}
}

#pragma region Private members
vector<Settlement*> Building::settlementsList;
bool Building::FindASettlement(Building* b, const bool _temporary)
{
	bool bSettlementDiscovered = false;

	if (b->IsPlaceable() == false) 
		return bSettlementDiscovered;

	if (b->bIsCentralBuilding == false)
	{
		const size_t numOfSettlements = Building::settlementsList.size();

		if (numOfSettlements == 0)
		{
			return bSettlementDiscovered;
		}
		for (unsigned int settlementsCounter = 0; settlementsCounter < numOfSettlements && bSettlementDiscovered == false; settlementsCounter++)
		{
			//Check if the building and the settlement belong to the same player and if the settlement is not indipendent.
			if ((b->GetPlayer() != Building::settlementsList[settlementsCounter]->GetPlayer())
				|| (Building::settlementsList[settlementsCounter]->IsIndipendent() == true))
			{
				return bSettlementDiscovered;
			}

			const vector<Building*> settBuildings = Building::settlementsList[settlementsCounter]->GetBuildingsBelongToSettlement();
			size_t numOfBuildings = settBuildings.size();
			for (unsigned int buildingsCounter = 0; buildingsCounter < numOfBuildings && bSettlementDiscovered == false; buildingsCounter++)
			{
				float b_xPos = b->get_xPos();
				float b_yPos = b->get_yPos();
				float xPos = settBuildings[buildingsCounter]->get_xPos();
				float yPos = settBuildings[buildingsCounter]->get_yPos();
				float distance = sqrt( pow(b_xPos - xPos, 2) + pow(b_yPos - yPos, 2) );

				//If the two buildings are close enough 
				bSettlementDiscovered = (distance <= MAX_DISTANCE);
			}
			if (bSettlementDiscovered == true)
			{
				//The settlement is valid
				if (_temporary == false)
				{
					b->settlement = Building::settlementsList[settlementsCounter];
					bSettlementDiscovered = bSettlementDiscovered = b->settlement->AddBuildingToSettlement(b); //Here, it should be return always true.
				}
			}
		}
	}
	else
	{
		bSettlementDiscovered = true;
		if (_temporary == false)
		{
			b->settlement = new Settlement(b->GetPlayer());
			Building::settlementsList.push_back(b->settlement);
			bSettlementDiscovered = b->settlement->AddBuildingToSettlement(b); //Here, it should be return always true.
		}
	}
	return bSettlementDiscovered;
}
#pragma endregion