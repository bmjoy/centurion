#include <game/strategy.h>
#include <classes/settlement.h>

using namespace std;


Settlement::Settlement()
{
	this->population = 10;
	this->max_population = 100;
	this->food = 1000;
	this->gold = 1000;
}

string Settlement::GetSettlementName(void)
{
	return this->name;
}

void Settlement::SetSettlementName(const string par_settlementName)
{
	this->name = par_settlementName;
}

bool Settlement::IsIndipendent(void)
{
	return this->bIsIndipendent;
}

unsigned short int Settlement::GetFood(void)
{
	return this->food;
}

void Settlement::SetFood(const unsigned short int food)
{
	this->food = food;
}

int Settlement::GetGold(void)
{
	return this->gold;
}

void Settlement::SetGold(const int gold)
{
	this->gold = gold;
}

unsigned short int Settlement::GetPopulation(void)
{
	return this->population;
}

void Settlement::SetPopulation(const unsigned short int population)
{
	this->population = population;
}

unsigned short int Settlement::GetMaxPopulation(void)
{
	return this->max_population;
}

void Settlement::SetMaxPopulation(const unsigned short int max_population)
{
	this->max_population = max_population;
}

vector<Building> Settlement::GetBuildingsBelongToSettlement(void)
{
	//To do
	return vector<Building>();
}

bool Settlement::AddBuildingToSettlement(Building& b)
{
	bool bAddCorrectly = false;
	//Adds a new building into the map only if it isn't already present.
	if(this->buildingsOfSettlement.count(b.GetPickingID()) == 0)
	{
		this->buildingsOfSettlement.insert(std::pair<unsigned int, Building>(b.GetPickingID(), b));
		bAddCorrectly = true;
	}
	return bAddCorrectly;
}

bool Settlement::RemoveBuildingFromSettlement(Building& b)
{
	bool bRemovedCorrectly = false;
	//The building can be removed only if it is present in the map.
	if (this->buildingsOfSettlement.count(b.GetPickingID()) > 0)
	{
		this->buildingsOfSettlement.erase(b.GetPickingID());
		bRemovedCorrectly = true;
	}
	return bRemovedCorrectly;
}

