#pragma once

#include <string>
#include "building.h"

#ifndef POPULATION_LIMIT
#define POPULATION_LIMIT   10000
#endif 

#ifndef GOLD_LIMIT
#define GOLD_LIMIT   1000000000
#endif 

#ifndef FOOD_LIMIT
#define FOOD_LIMIT   1000000000
#endif 

using namespace std;

class Settlement
{
public:
	string GetSettlementName(void);
	void SetSettlementName(const string par_settlementName);
	bool IsIndipendent(void);

	unsigned int GetFood(void);
	void SetFood(const unsigned int par_food);
	int GetGold(void);
	void SetGold(int par_gold);
	unsigned int GetPopulation(void);
	void SetPopulation(const unsigned int par_population);
	unsigned int GetMaxPopulation(void);
	void SetMaxPopulation(const unsigned int par_max_population);

	vector<Building> GetBuildingsBelongToSettlement(void);
	bool AddBuildingToSettlement(Building& b);
	bool RemoveBuildingFromSettlement(Building& b);

	Settlement();
	~Settlement() {};
private:
	bool bIsIndipendent = false;
	string name;
	unsigned int food;
	int gold;
	unsigned int population;
	unsigned int max_population;
	map<unsigned int, Building> buildingsOfSettlement;
};