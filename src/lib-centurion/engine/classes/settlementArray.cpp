#include "settlementArray.h"

using namespace std;
using namespace glm;

namespace SettlementArray
{
	// Private variables 
	namespace
	{
		std::vector<Settlement*> settlementsList;
	};

	void SettlementArray::ResetSettlementsList(void)
	{
		settlementsList.clear();
	}

	std::vector<Settlement*>* SettlementArray::GetSettlementListPtr(void)
	{
		return &(SettlementArray::settlementsList);
	}

	Settlement* SettlementArray::GetSettlementById(const unsigned int id)
	{
		if (id >= settlementsList.size()) return nullptr;
		return settlementsList[id];
	}

	void SettlementArray::PushSettlement(Settlement* settlPtr)
	{
		settlementsList.push_back(settlPtr);
	}

	void EraseSettlement(Settlement* settlPtr)
	{
		SettlementArray::settlementsList.erase(std::remove(SettlementArray::settlementsList.begin(), SettlementArray::settlementsList.end(), settlPtr), SettlementArray::settlementsList.end());
	}

	unsigned int Size(void)
	{
		return (unsigned int)SettlementArray::settlementsList.size();
	}


};