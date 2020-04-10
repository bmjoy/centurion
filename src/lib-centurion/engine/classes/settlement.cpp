#include <game/strategy.h>
#include <classes/settlement.h>

using namespace std;
using namespace glm;


Settlement::Settlement(const unsigned int par_player)
{
	this->player = par_player;
	this->scriptName = "";
	this->bIsIndipendent = false;
	//READ THOSE VALUES FROM SETTLEMENT.OC.XML
	this->population = 10;
	this->max_population = 100;
	this->food = 1000;
	this->gold = 1000;
}

string Settlement::GetSettlementName(void)
{
	return this->scriptName;
}

void Settlement::SetSettlementName(const string par_settlementName)
{
	this->scriptName = par_settlementName;
}

bool Settlement::IsIndipendent(void)
{
	return this->bIsIndipendent;
}

unsigned int Settlement::GetFood(void)
{
	return this->food;
}

void Settlement::SetFood(const unsigned int par_food)
{
	this->food = par_food <= FOOD_LIMIT ? par_food : FOOD_LIMIT;
}

int Settlement::GetGold(void)
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

unsigned int Settlement::GetPopulation(void)
{
	return this->population;
}

void Settlement::SetPopulation(const unsigned int par_population)
{
	this->population = par_population <= POPULATION_LIMIT ? par_population : POPULATION_LIMIT;
	if (this->population > this->max_population)
	{
		this->population = this->max_population;
	}
}

unsigned int Settlement::GetMaxPopulation(void)
{
	return this->max_population;
}

void Settlement::SetMaxPopulation(const unsigned int par_max_population)
{
	this->max_population = par_max_population <= POPULATION_LIMIT ? par_max_population : POPULATION_LIMIT;
}

unsigned int Settlement::GetPlayer(void)
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
