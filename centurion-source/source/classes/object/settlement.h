#pragma once

#include <string>
#include "building.h"

using namespace std;

class Settlement
{
public:
	string GetSettlementName(void);
	void SetSettlementName(const string par_settlementName);
	Settlement() {};
	~Settlement() {};
private:
	string name;
};