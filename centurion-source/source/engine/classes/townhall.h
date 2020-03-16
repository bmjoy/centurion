#ifndef _TOWNHALL_H
#define _TOWNHALL_H

#include <ui.h>
#include "Stronghold.h"

class Unit;

using namespace std;
using namespace glm;


class Townhall : public StrongholdBuilding
{
public:
protected:
	Townhall();
private:
	vector<Unit> units;
};

#endif