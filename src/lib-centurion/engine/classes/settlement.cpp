#include <game/strategy.h>
#include <classes/settlement.h>
#include "classes-data.h"

using namespace std;
using namespace glm;


Settlement::Settlement(const unsigned int par_player, glm::vec3 * pos): player(par_player), position(pos)
{
	this->idName = "";
	this->bIsIndipendent = false;
	this->SetSettlementProperties();
}

Settlement::Settlement()
{
	this->idName = "";
	this->bIsIndipendent = false;
	this->SetSettlementProperties();
}

Settlement::~Settlement(void)
{
}

string Settlement::GetSettlementName(void) const
{
	return this->idName;
}

void Settlement::SetSettlementName(const string par_settlementName)
{
	this->idName = par_settlementName;
}

bool Settlement::IsIndipendent(void) const
{
	return this->bIsIndipendent;
}

unsigned int Settlement::GetFood(void) const
{
	return this->food;
}

void Settlement::SetFood(const unsigned int par_food)
{
	this->food = par_food <= FOOD_LIMIT ? par_food : FOOD_LIMIT;
}

int Settlement::GetGold(void) const
{
	return this->gold;
}

void Settlement::SetGold(int par_gold)
{
	if (par_gold >= 0)
	{
		this->gold = par_gold <= GOLD_LIMIT ? par_gold : GOLD_LIMIT;
	}
	else
	{
		this->gold = par_gold >= (-GOLD_LIMIT) ? par_gold : (-GOLD_LIMIT);
	}
}

unsigned int Settlement::GetPopulation(void) const
{
	return this->population;
}

void Settlement::SetPopulation(const unsigned int par_population)
{
	this->population = par_population <= POPULATION_LIMIT ? par_population : POPULATION_LIMIT;
	if (this->population > this->maxPopulation)
	{
		this->population = this->maxPopulation;
	}
}

unsigned int Settlement::GetMaxPopulation(void) const
{
	return this->maxPopulation;
}

void Settlement::SetMaxPopulation(const unsigned int par_max_population)
{
	this->maxPopulation = par_max_population <= POPULATION_LIMIT ? par_max_population : POPULATION_LIMIT;
}

unsigned int Settlement::GetPlayer(void) const
{
	return this->player;
}

void Settlement::SetPlayer(const unsigned int par_player)
{
	if (par_player != this->player)
	{
		this->player = par_player;
		for (map<unsigned int, Building*>::iterator mapIterator = this->buildingsOfSettlement.begin(); mapIterator != this->buildingsOfSettlement.end(); mapIterator++)
		{
			mapIterator->second->SetPlayer(par_player);
		}
	}
}

glm::vec3 Settlement::GetPosition(void) const
{
	return (*this->position);
}

void Settlement::SetPosition(glm::vec3* pos)
{
	position = pos;
}

const vector<Building*> Settlement::GetBuildingsBelongToSettlement(void)
{
	vector<Building*> list;
	for (map<unsigned int, Building*>::iterator mapIterator = this->buildingsOfSettlement.begin(); mapIterator != this->buildingsOfSettlement.end(); mapIterator++)
	{
		list.push_back(mapIterator->second);
	}
	return list;
}

bool Settlement::AddBuildingToSettlement(Building* b)
{
	bool bAddCorrectly = false;
	//Adds a new building into the map only if it isn't already present.
	if(this->buildingsOfSettlement.count(b->GetPickingID()) == 0)
	{
		this->buildingsOfSettlement.insert(std::pair<unsigned int, Building*>(b->GetPickingID(), b));
		bAddCorrectly = true;
	}
	return bAddCorrectly;
}

bool Settlement::RemoveBuildingFromSettlement(Building* b)
{
	bool bRemovedCorrectly = false;
	//The building can be removed only if it is present in the map.
	if (this->buildingsOfSettlement.count(b->GetPickingID()) > 0)
	{
		this->buildingsOfSettlement.erase(b->GetPickingID());
		bRemovedCorrectly = true;
	}
	return bRemovedCorrectly;
}

#pragma region Private members:
void Settlement::SetSettlementProperties(void)
{
	int iProperty = 0;

	ClassesData::XMLClassData setData = *ClassesData::GetClassesData("Settlement");
	ClassesData::SetFixedPtr(&setData);
	setData.GetParentData(setData.GetParentClass());
	ClassesData::SetFixedPtr(nullptr);

	ClassesData::TryParseInteger(setData.GetPropertiesMap(), "gold", &iProperty);
	this->gold = iProperty;
	ClassesData::TryParseInteger(setData.GetPropertiesMap(), "food", &iProperty);
	this->food = iProperty;
	ClassesData::TryParseInteger(setData.GetPropertiesMap(), "maxPopulation", &iProperty);
	this->maxPopulation = iProperty;
}
#pragma endregion
