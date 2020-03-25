/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include "object.h"

using namespace std;
using namespace glm;

class Building;
class Player;

/// <summary>
/// (???) Da rivedere e in parte da riscrivere. 
/// </summary>
class Decoration : public GObject
{
public:
	/// <summary>
	/// Public constructor.
	/// </summary>
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

