#pragma once

#include <ui.h>
#include <json.hpp>
#include "object.h"
#include "settlement.h"

using namespace std;
using namespace glm;

class Unit;
namespace game { class ObjectUI; };

class Player;

class Settlement;

class Building : public GObject
{
public:
	Settlement GetSettlement(void);
	void SetSettlement(const Settlement par_settlement);

	bool IsClickableInMimimap(void);
	void CanBeClickableInMimimap(const bool par_clickable);

	bool IsPlaceable();
	void SetPlaceable(const bool placeable);

	string GetCategory();


	void prepare() override;
	void create(string Name = "");
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	void SetStatus(const bool bIsCreated);

	void setWaitingToBeErased(bool b) { waitingToBeErased = b; }
	bool getWaitingToBeErased() { return waitingToBeErased; }

	Building();
	~Building();
	
private:
	game::ObjectUI* buildingUI;
	map<int, Building*> subs_buildings; // dependent buildings connected to indipendent one
	vector<Unit> unitsInside;
	bool waitingToBeErased;
	bool bIsCreated;
	size_t buildingListSize;
	bool bIsPlaceable;
	bool bIsClickableInMimimap;
	gui::Circle circle[2];
	string category;
	string ent_path;
	string pass_path;
	bool bIsTownhall;
	bool bIsVillagehall;
	bool bIsCentralBuilding = false;
	Settlement settlement;

	// properties from sprite
	float sprite_width, sprite_height;
	GLuint textureID;

	vec2 getSpriteSize(string ent_path);
	//sound selectionSound; TODO
};

//struct buildingProperties 
/*
{
	// properties from class 
	string type;
	string race;
	string class_name;
	string category;
	string ent_path;
	string pass_path;
	float radius;
	bool is_indipendent;
	bool clickable_in_minimap;
	bool is_townhall;
	bool is_villagehall;

	// properties from sprite
	float sprite_width, sprite_height;
	GLuint textureID;
};

class Building : public GObject
{
public:
	Building();
	void prepare() override;
	void create(string Name = "");
	void render(bool picking, int clickID = 0, bool not_placeable = false) override;
	bool is_placeable();
	string get_race() { return prop.race; }
	void set_placeable(bool b) { isPlaceable = b; }
	void set_status(bool b) { isCreated = b; }
	bool is_independent() { return prop.is_indipendent; }
	void set_settlement_name(string s) { settl_name = s; }
	string get_settlement_name() { return settl_name; }
	void set_settlement_building(Building *b) { independent = b; }
	Building *get_settlement_building() { return independent; }
	bool is_near_to_independent(string *Category);
	int UnitsInBuilding();
	vector<Unit> UnitsInHolder();
	vector<int> buildingsInSettlementIds();
	int buildingsInSettlementCount() { return (int)subs_buildings.size(); }
	void setWaitingToBeErased(bool b) { waitingToBeErased = b; }
	bool getWaitingToBeErased() { return waitingToBeErased; }
	string getCategory() { return prop.category; }
	~Building();

private:
	game::ObjectUI* buildingUI;
	vector<Unit> unitsInside;
	map<int, Building*> subs_buildings; // dependent buildings connected to indipendent one
	buildingProperties prop;
	string settl_name;
	Building *independent;
	bool waitingToBeErased;
	bool isCreated;
	size_t buildingListSize;
	bool isPlaceable;
	gui::Circle circle[2];
	vec2 getSpriteSize(string ent_path);
	//sound selectionSound; TODO
};
*/