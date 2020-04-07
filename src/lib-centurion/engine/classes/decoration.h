/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include "object.h"

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
	void Render(const bool picking, const unsigned int clickID = 0) override;
	Building *get_settlement_building() { return independent; }
	std::string get_settlement_name() { return settlementName; }
	~Decoration();

protected:
	void CheckIfPlaceable(void) override;
private:
	std::string settlementName;
	float oldX, oldY, oldSettlX, oldSettlY;
	int settlID;
	void checkSettlement();
	Building* independent;
};

