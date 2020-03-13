#pragma once

#include <gui>
#include <json.hpp>

using namespace std;
using namespace glm;

class Unit;
class Building;
class Decoration;

class Player;


//
//	GAME OBJECT --> source/object.cpp
//


class GObject
{
public:
	//-- Methods
	unsigned short int GetPlayer(void);
	void SetPlayer(const unsigned short int par_playerID);
	bool IsSelected(void);
	void Select(bool par_selected);
	unsigned int GetPickingID(void);
	void SetPickingID(const unsigned int par_pickingID);
	unsigned short int GetRadius(void);
	void SetRadius(const unsigned short int par_radius);
	string GetClassName(void);
	void SetClassName(const string par_className);
	string GetType(void);
	void SetType(const string par_type);
	string GetName(void);
	void SetName(const string par_name);
	unsigned short int GetRace(void);
	void SetRace(const unsigned short int par_race);
	unsigned short int GetSight(void);
	void SetSight(const unsigned short int par_sight);
	bool CanBePositionedIntoWater(void);
	void AllowPositioningIntoWater(void);
	void DenyPositioningIntoWater(void);
	bool IsBuilding(void);
	bool IsUnit(void);
	bool IsDecoration(void);
	Building* AsBuilding(void);
	Unit* AsUnit(void);
	Decoration* AsDecoration(void);
	void SetPosition(const vec3 pos);
	vec3 GetPosition(void);
	int get_xPos(void);
	int get_yPos(void);
		
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
	unsigned short int radius;
	vec3 position;
	string className;
	string name;
	string type;
	bool selected = false;
	bool isWaterObject = false;
};
