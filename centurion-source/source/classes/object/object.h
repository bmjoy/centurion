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
	unsigned short int GetPlayer(void);
	void SetPlayer(const unsigned short int par_playerID);
	bool IsSelected(void);
	void Select(bool par_selected);
	unsigned int GetPickingID(void);
	void SetPickingID(const unsigned int par_pickingID);
	string GetClassName(void);
	void SetClassName(const string par_className);
	string GetType(void);
	void SetType(const string par_type);
	string GetName(void);
	void SetName(const string par_name);


	void set_position(vec3 pos);
	vec3 get_position() { return position; }
	
	
	bool isSelected() { return selected; }
	int get_xPos() { return (int)position.x; }
	int get_yPos() { return (int)position.y; }
	
	
	void update_pass();
	void clear_pass();

	Building* AsBuilding() { return (Building*)this; }
	Unit* AsUnit() { return (Unit*)this; }
	Decoration* AsDecoration() { return (Decoration*)this; }
	bool IsBuilding() { return (type == "building"); }
	bool IsUnit() { return (type == "unit"); }
	bool IsDecoration() { return (type == "decoration"); }

	virtual void prepare() { }
	virtual void create() { }
	virtual void render(bool picking, int clickID = 0, bool not_placeable = false) {};

	~GObject();
protected:
	GObject();
	Player *player;
	vec3 pickingColor;
	json data;
	vec3 position;
	vector<vector<int>> pass_grid;
private:
	unsigned short int playerID;
	unsigned int pickingID;
	string className;
	string name;
	string type;
	bool selected = false;
};

/*
class GObject 
{
public:
	GObject();
	void set_position(vec3 pos);
	vec3 get_position() { return position; }
	void set_class(string class_name);
	void set_player(int player_id);
	int get_playerID() { return playerID; }
	void set_id(int pickingId);
	int GetPickingID() { return picking_id; }
	bool isSelected() { return selected; }
	int get_xPos() { return (int)position.x; }
	int get_yPos() { return (int)position.y; }
	string get_class() { return className; }
	string get_name() { return name; }
	void set_name(string Name) { name = Name; }
	string get_type() { return type; }
	void set_type(string Type) { type = Type; }
	void update_pass();
	void clear_pass();

	Building* AsBuilding() { return (Building*)this; }
	Unit* AsUnit() { return (Unit*)this; }
	Decoration* AsDecoration() { return (Decoration*)this; }
	bool IsBuilding() { return (type == "building"); }
	bool IsUnit() { return (type == "unit"); }
	bool IsDecoration() { return (type == "decoration"); }

	virtual void prepare() { }
	virtual void create() { }
	virtual void render(bool picking, int clickID = 0, bool not_placeable = false) {};

	~GObject();
protected:
	Player *player;
	int playerID;
	vec3 pickingColor;
	bool selected;
	json data;
	vec3 position;
	string className;
	int picking_id;
	string name, type;
	vector<vector<int>> pass_grid;
};
*/


