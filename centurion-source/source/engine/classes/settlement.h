#pragma once

#include <string>
#include "building.h"

using namespace std;

class Settlement
{
public:
	string GetSettlementName(void);
	void SetSettlementName(const string par_settlementName);
	bool IsIndipendent(void);

	unsigned short int GetFood(void);
	void SetFood(unsigned short int food);
	int GetGold(void);
	void SetGold(int gold);
	unsigned short int GetPopulation(void);
	void SetPopulation(unsigned short int population);
	unsigned short int GetMaxPopulation(void);
	void SetMaxPopulation(unsigned short int max_population);

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
	unsigned short int population;
	unsigned short int max_population;
	map<unsigned int, Building> buildingsOfSettlement;
};