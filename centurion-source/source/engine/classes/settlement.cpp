#include <game/strategy.h>
#include <classes/settlement.h>

using namespace std;


Settlement::Settlement()
{
	//READ THOSE VALUES FROM SETTLEMENT.OC.XML
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

void Settlement::SetFood(unsigned short int food)
{
	if (food < 0) {
		this->food = 0;
	}

	//TRANSFORM THE VALUE INTO A CONSTANT
	if (food > 999999)
		this->food = (unsigned short int)999999;
	else
		this->food = food;
}

int Settlement::GetGold(void)
{
	return this->gold;
}

void Settlement::SetGold(int gold)
{
	//LOAN OR NOT LESS THAN 0?
	//TRANSFORM THE VALUE INTO A CONSTANT
	if (gold > 999999)
		this->gold = 999999;
	else
		this->gold = gold;
}

unsigned short int Settlement::GetPopulation(void)
{
	return this->population;
}

void Settlement::SetPopulation(unsigned short int population)
{
	if (population < 0) {
		this->population = 0;
	}

	if (population > GetMaxPopulation())
		this->population = GetMaxPopulation();
	else
		this->population = population;
}

unsigned short int Settlement::GetMaxPopulation(void)
{
	return this->max_population;
}

void Settlement::SetMaxPopulation(unsigned short int max_population)
{
	if (max_population < 0) {
		this->max_population = 0;
	}

	//TRANSFORM THE VALUE INTO A CONSTANT
	if (max_population > 5000)
		this->max_population = (unsigned short int)5000;
	else
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

