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
