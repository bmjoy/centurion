#pragma once

#include <ui.h>
#include "object.h"
#include <json.hpp>

using namespace std;
using namespace glm;

class Building;
class Player;

class Decoration : public GObject
{
public:
	Decoration();
	void prepare() override;
	bool is_placeable();
	void create();
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	Building *get_settlement_building() { return independent; }
	string get_settlement_name() { return settlementName; }
	~Decoration();

private:
	string settlementName;
	float oldX, oldY, oldSettlX, oldSettlY;
	int settlID;
	void checkSettlement();
	Building* independent;
};

