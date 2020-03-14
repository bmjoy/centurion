#pragma once

#include <gui>
#include <json.hpp>

using namespace std;
using namespace glm;

class Unit;
class Building;
class Decoration;

class Player;

#ifndef UNDEFINED_RACE
#define UNDEFINED_RACE    0
#endif 
#ifndef ROMAN_RACE
#define ROMAN_RACE    1
#endif 
#ifndef EGYPTIAN_RACE
#define EGYPTIAN_RACE    2
#endif 

//
//	GAME OBJECT --> source/object.cpp
//


class GObject
{
public:
	//Player methods
	unsigned short int GetPlayer(void);
	void SetPlayer(const unsigned short int par_playerID);

	//Selection methods
	bool IsSelected(void);
	void Select(bool par_selected);

	//Picking ID methods 
	unsigned int GetPickingID(void);
	void SetPickingID(const unsigned int par_pickingID);

	//Radius methods
	float GetRadius(void);
	void SetRadius(const float par_radius);

	//Class name methods
	string GetClassName(void);
	void SetClassName(const string par_className);

	//Object name methods
	string GetName(void);
	void SetName(const string par_name);

	//Race methods
	unsigned short int GetRace(void);
	void SetRace(const unsigned short int par_race);
	string GetRaceName(void);
	void SetRaceName(const string par_raceName);

	//Sight methods
	unsigned short int GetSight(void);
	void SetSight(const unsigned short int par_sight);

	//Positioing into water methods
	bool CanBePositionedIntoWater(void);
	void AllowPositioningIntoWater(void);
	void DenyPositioningIntoWater(void);

	//Type methods
	string GetType(void);
	void SetType(const string par_type);
	bool IsBuilding(void);
	bool IsUnit(void);
	bool IsDecoration(void);

	//Casting methods
	Building* AsBuilding(void);
	Unit* AsUnit(void);
	Decoration* AsDecoration(void);

	//Position methods
	void SetPosition(const vec3 pos);
	vec3 GetPosition(void);
	int get_xPos(void);
	int get_yPos(void);
	
	//Pass methods
	void update_pass(void);
	void clear_pass(void);

	virtual void prepare(void) { };
	virtual void create(void) { };
	virtual void render(bool picking, int clickID = 0, bool not_placeable = false) {};

	~GObject();
protected:
	GObject();
	Player *player;
	vec3 pickingColor;
	json data;
	vector<vector<int>> pass_grid;
private:
	unsigned short int playerID;
	unsigned int pickingID;
	unsigned short int race;
	unsigned short int sight;
	float radius;
	vec3 position;
	string raceName;
	string className;
	string name;
	string type;
	bool selected = false;
	bool isWaterObject = false;
};
