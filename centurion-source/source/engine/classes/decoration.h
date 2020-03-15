#pragma once

#include <gui>
#include "object.h"
#include <json.hpp>

using namespace std;
using namespace glm;

class Building;
class Player;

//
//	GAME OBJECT --> source/object.cpp
//


class Decoration : public GObject
{
public:
	Decoration();
	void prepare() override;
	bool is_placeable();
	void create() override;
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	Building *get_settlement_building() { return independent; }
	string get_settlement_name() { return settlementName; }
	~Decoration();

private:
	DecorationData decorData;
	string settlementName;
	float oldX, oldY, oldSettlX, oldSettlY;
	int settlID;
	void checkSettlement();
	Building* independent;
};

