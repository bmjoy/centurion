#ifndef _HABITABLE_H
#define _HABITABLE_H


#include <ui.h>
#include "building.h"

using namespace std;
using namespace glm;


class Habitable : public Building
{
protected:
	void StartGoldProduction(void);
	void StopGoldProduction(void);
	void StartFoodProduction(void);
	void StopFoodProduction(void);
private:
	bool bCanProduceGold = false;
	bool bCanProduceFood = false;
};

#endif // !