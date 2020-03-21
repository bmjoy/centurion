#pragma once

#include <ui.h>
#include <json.hpp>
#include <object_sprite.h>
#include "object-data.h"

using namespace std;
using namespace glm;

class Unit;
class Building;
class Decoration;

class Player;

#ifndef MAX_NUMBER_OF_OBJECTS
#define MAX_NUMBER_OF_OBJECTS 100
#endif

#ifndef UNDEFINED_RACE
#define UNDEFINED_RACE    0
#endif 
#ifndef ROMAN_RACE
#define ROMAN_RACE    1
#endif 
#ifndef EGYPTIAN_RACE
#define EGYPTIAN_RACE    2
#endif 


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
	float GetSelectionRadius(void);
	void SetSelectionRadius(const float par_selectionRadius);

	//Class name methods
	string GetClassName(void);
	void SetClassName(const string par_className);

	//Object name methods
	void SetSingularName(const string par_singularName);
	string GetSingularName(void);
	void SetPluralName(const string par_pluralName);
	string GetPluralName(void);

	//Race methods
	unsigned int GetRace(void);
	void SetRace(const unsigned short int par_race);
	string GetRaceName(void);
	void SetRaceName(const string par_raceName);

	//Sight methods
	unsigned int GetSight(void);
	void SetSight(const unsigned short int par_sight);

	//Positioning into water methods
	bool CanBePositionedIntoWater(void);
	void AllowPositioningIntoWater(void);
	void DenyPositioningIntoWater(void);

	//Duplication into editor methods
	bool GetCanBeClonedInEditor(void);
	void SetCanBeClonedInEditor(const bool par_canBeClonedInEditor);

	//Visibility into minimap:
	bool GetAlwaysVisibleInGameMinimap(void);
	void SetAlwaysVisibleInGameMinimap(const bool par_alwaysVisibleInGameMinimap);

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
	float get_xPos(void);
	float get_yPos(void);
	
	//Pass methods
	void update_pass(void);
	void clear_pass(void);

	//Others methods
	virtual void prepare(void) { };
	bool Create(const string _className);
	virtual void render(bool picking, int clickID = 0, bool not_placeable = false) {};

	//Static methods
	static unsigned int GetNumberOfObjects(void);
	static unsigned int GetNumberOfBuildings(void);
	static unsigned int GetNumberOfUnits(void);
	static unsigned int GetNumberOfDecorations(void);

	static void AddGameObject(const unsigned int index, GObject* object);
	static void RemoveGameObject(const unsigned int index);
	static void ResetGameObjects(void);
	static GObject* GetObjectByID(const unsigned int ID);

	static vector<Building*> GetListOfBuildings(void);
	static vector<Unit*> GetListOfUnits(void);
	static vector<Decoration*> GetListOfDecorations(void);

	GObject();

	~GObject();
protected:
	ObjectSprite::SpriteData spriteData;
	Player *player;
	vec3 pickingColor;
	json data;
	vector<vector<int>> pass_grid;
	map<string, string> methods;
	map<string, string> sounds;
private:
	unsigned short int playerID;
	unsigned int pickingID;
	unsigned int race;
	unsigned int sight;
	float selectionRadius;
	float radius;
	vec3 position;
	string singularName;
	string pluralName;
	string raceName;
	string className;
	string type;
	bool bSelected = false;
	bool bIsWaterObject = false;
	bool canBeClonedInEditor = false;
	bool bAlwaysVisibleInGameMinimap = false;

	//Properties for static methods:
	static unsigned int numberOfObjects;
	static unsigned int numberOfBuildings;
	static unsigned int numberOfUnits;
	static unsigned int numberOfDecorations;
	static GObject* GameObjects[MAX_NUMBER_OF_OBJECTS];

	//Private members:
	void SetObjectProperties(ObjectData::ObjectXMLClassData &objData);
};
