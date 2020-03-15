#include <game/strategy.h>
#include <object/settlement.h>

using namespace std;


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