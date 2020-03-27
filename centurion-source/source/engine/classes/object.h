/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

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

/// <summary>
/// (???) Tutte costanti da rivedere.
/// </summary>
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
	/// <summary>
	/// This function returns the player of the current object.
	/// </summary>
	/// <returns>The number of the player.</returns>
	unsigned short int GetPlayer(void);
	/// <summary>
	/// (???) Da rivedere se l'object è una struttura.
	/// This function sets the player of current the object.
	/// </summary>
	/// <param name="par_playerID"></param>
	void SetPlayer(const unsigned short int par_playerID);

	/// <summary>
	/// This function checks if the current object is selected.
	/// </summary>
	/// <returns>True if the object is selected; false otherwise.</returns>
	bool IsSelected(void);
	/// <summary>
	/// (???) Da rivedere.
	/// This function marks the current object as selected (true) or not (false).
	/// </summary>
	/// <param name="par_selected"></param>
	void Select(bool par_selected);

	/// <summary>
	/// This function retrieves the unique picking ID of the current object.
	/// </summary>
	/// <returns></returns>
	unsigned int GetPickingID(void);
	/// <summary>
	/// (???) Da cancellare, forse.
	/// </summary>
	/// <param name="par_pickingID"></param>
	void SetPickingID(const unsigned int par_pickingID);

	/// <summary>
	/// This function returns the radius of the current object.
	/// </summary>
	/// <returns>The radius of the current object.</returns>
	float GetRadius(void);
	/// <summary>
	/// This function sets the radius of the current object.
	/// </summary>
	/// <param name="par_radius">The radius of the current object. It should be a value greater than 0.</param>
	void SetRadius(const float par_radius);
	/// <summary>
	/// (???) Da cancellare perché un duplicato -> Da cancellare soprattutto nei vari XML.
	/// </summary>
	/// <returns></returns>
	float GetSelectionRadius(void);
	void SetSelectionRadius(const float par_selectionRadius);

	/// <summary>
	/// This function returns the class name of the current object.
	/// </summary>
	/// <returns>The class name of the current object.</returns>
	string GetClassName(void);
	/// <summary>
	/// This function sets the class name of the current object.
	/// </summary>
	/// <param name="par_className">The class name.</param>
	void SetClassName(const string par_className);

	/// <summary>
	/// This function sets the name of the current object that will be shown when a single object is selected.
	/// </summary>
	/// <param name="par_singularName">The singular name of the object.</param>
	void SetSingularName(const string par_singularName);
	/// <summary>
	/// This function returns the name of the current object that will be shown when a single object is selected.
	/// </summary>
	/// <returns>The singular name of the object.</returns>
	string GetSingularName(void);
	/// <summary>
	/// This function sets the prural of the current object that will be shown when more objects belong to the same class are selected at the same time.
	/// </summary>
	/// <param name="par_pluralName">The prural name.</param>
	void SetPluralName(const string par_pluralName);
	/// <summary>
	/// This function returns the prural of the current object that will be shown when more objects belong to the same class are selected at the same time.
	/// </summary>
	/// <returns>The prural name.</returns>
	string GetPluralName(void);

	/// <summary>
	/// This function returns the race ID of the current object.
	/// </summary>
	/// <returns>The race Id of the current object.</returns>
	unsigned int GetRace(void);
	/// <summary>
	/// (???) Da rivedere --> bisogna fare una corrispondenza nome razza ID (es. Rome = 0, Egypt = 1, ecc).
	/// This function sets the race ID of the current object.
	/// </summary>
	/// <param name="par_race">The race ID.</param>
	void SetRace(const unsigned short int par_race);
	/// <summary>
	/// This function return the race name of the current object.
	/// </summary>
	/// <returns>The race name of the current object.</returns>
	string GetRaceName(void);
	/// <summary>
	/// This function sets the race name of the current object.
	/// </summary>
	/// <param name="par_raceName">The race name.</param>
	void SetRaceName(const string par_raceName);

	/// <summary>
	/// This function returns the sight og the current object.
	/// </summary>
	/// <returns>The sight of the current object.</returns>
	unsigned int GetSight(void);
	/// <summary>
	/// This function sets the sight of the current object.
	/// </summary>
	/// <param name="par_sight">The sight of the current object. i t should be a value greater than 0.</param>
	void SetSight(const unsigned short int par_sight);

	/// <summary>
	/// This function checks if the current object can be positioned into water or not.
	/// </summary>
	/// <returns>True if it can be positionated; false otherwise.</returns>
	bool CanBePositionedIntoWater(void);
	/// <summary>
	/// This function allows to place the current object into water.
	/// </summary>
	void AllowPositioningIntoWater(void);
	/// <summary>
	/// This function denies to place the current object into water. 
	/// </summary>
	void DenyPositioningIntoWater(void);

	/// <summary>
	/// This function checks if the the current object can be cloned in the editor.
	/// </summary>
	/// <returns>True if it can be cloned; false otherwise.</returns>
	bool GetCanBeClonedInEditor(void);
	/// <summary>
	/// This function sets if the current object can be cloned in the editor.
	/// </summary>
	/// <param name="par_canBeClonedInEditor">True or false</param>
	void SetCanBeClonedInEditor(const bool par_canBeClonedInEditor);

	/// <summary>
	/// This function checks if the current object is alway visibile in the game minimap.
	/// </summary>
	/// <returns>True if it is always visible; false otherwise.</returns>
	bool GetAlwaysVisibleInGameMinimap(void);
	/// <summary>
	/// This function sets if the current object is always visible in the game minimap.
	/// </summary>
	/// <param name="par_alwaysVisibleInGameMinimap">True or false.</param>
	void SetAlwaysVisibleInGameMinimap(const bool par_alwaysVisibleInGameMinimap);

	/// <summary>
	/// This function return the type of the current object.
	/// </summary>
	/// <returns>The type of the current object.</returns>
	string GetType(void);
	/// <summary>
	/// This function sets the type of the current object.
	/// </summary>
	/// <param name="par_type">The type of the current object.</param>
	void SetType(const string par_type);

	/// <summary>
	/// This function check if the current object belong to the building classes family or not.
	/// </summary>
	/// <returns>True or not.</returns>
	bool IsBuilding(void);
	/// <summary>
	/// This function check if the current object belong to the unit classes family or not.
	/// </summary>
	/// <returns>True or false.</returns>
	bool IsUnit(void);
	/// <summary>
	/// This function check if the current object belong to the building classes family or not.
	/// </summary>
	/// <returns>True or false.</returns>
	bool IsDecoration(void);

	/// <summary>
	/// This function performs the cast into a building of the current object.
	/// </summary>
	/// <returns>A pointer to a building.</returns>
	Building* AsBuilding(void);
	/// <summary>
	/// This function performs the cast into an unit of the current object.
	/// </summary>
	/// <returns>A pointer to an unit.</returns>
	Unit* AsUnit(void);
	/// <summary>
	/// This function performs the cast into a decoration of the current object.
	/// </summary>
	/// <returns>A pointer to a decoration</returns>
	Decoration* AsDecoration(void);

	/// <summary>
	/// (???) Da rivedere --> Inventare una classe Point?
	/// This function sets the position of the current object.
	/// </summary>
	/// <param name="pos">The position (x and y)</param>
	void SetPosition(const vec3 pos);
	/// <summary>
	/// This function returns the position of the current object.
	/// </summary>
	/// <returns></returns>
	vec3 GetPosition(void);
	/// <summary>
	/// This function returns only the x position of the current object.
	/// </summary>
	/// <returns>The x position.</returns>
	float get_xPos(void);
	/// <summary>
	/// This function return the y position of the current object.
	/// </summary>
	/// <returns>The y position.</returns>
	float get_yPos(void);
	
	//Pass methods
	void update_pass(void);
	void clear_pass(void);

	/// <summary>
	/// (???)
	/// </summary>
	virtual void prepare(void) { };
	/// <summary>
	/// This function create a specific object.
	/// </summary>
	/// <param name="_className">The class of the object.</param>
	/// <returns>Trye if the object has been created; false otherwise.</returns>
	bool Create(const string _className);
	/// <summary>
	/// This function perfoms the rendering of the current object. It must be overread.
	/// </summary>
	virtual void render(bool picking, int clickID = 0, bool not_placeable = false) {};

	#pragma region Static members
	/// <summary>
	/// This function return the total number of objects that the game has been created.
	/// </summary>
	/// <returns>The total number of objects.</returns>
	static unsigned int GetNumberOfObjects(void);
	/// <summary>
	/// This function return the total number of buildings that the game has been created.
	/// </summary>
	/// <returns>The total number of buildings.</returns>
	static unsigned int GetNumberOfBuildings(void);
	/// <summary>
	/// This function return the total number of units that the game has been created.
	/// </summary>
	/// <returns>The total number of units.</returns>
	static unsigned int GetNumberOfUnits(void);
	/// <summary>
	/// This function return the total number of decorations that the game has been created.
	/// </summary>
	/// <returns>The total number of decorations.</returns>
	static unsigned int GetNumberOfDecorations(void);

	/// <summary>
	/// This function adds an object to the game.
	/// </summary>
	/// <param name="index">The index of the object.</param>
	/// <param name="object">A pointer to the object.</param>
	static void AddGameObject(const unsigned int index, GObject* object);
	/// <summary>
	/// This function removes an object from the game.
	/// </summary>
	/// <param name="index">The index of the objet that is waiting for its removal.</param>
	static void RemoveGameObject(const unsigned int index);
	/// <summary>
	/// This function deletes all the objects from the game.
	/// </summary>
	static void ResetGameObjects(void);
	/// <summary>
	/// This functions returns an existing object by its unique picking ID.
	/// </summary>
	/// <param name="ID">The picking ID of the object.</param>
	/// <returns></returns>
	static GObject* GetObjectByID(const unsigned int ID);

	/// <summary>
	/// This function return a list of all the buildings of the game.
	/// </summary>
	/// <returns>A list of buildings.</returns>
	static vector<Building*> GetListOfBuildings(void);
	/// <summary>
	/// This function returns a list of all the units of the game.
	/// </summary>
	/// <returns>A list of units.</returns>
	static vector<Unit*> GetListOfUnits(void);
	/// <summary>
	/// This function returns a list of all the decoration of the game.
	/// </summary>
	/// <returns>A list of decoration</returns>
	static vector<Decoration*> GetListOfDecorations(void);
	#pragma endregion

	/// <summary>
	/// Public constructor.
	/// </summary>
	GObject();
	/// <summary>
	/// The destructor.
	/// </summary>
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

	#pragma region Private members
	/// <summary>
	/// This functions sets the properties of the current object when it is being created. 
	/// The values of these properties were previously read 
	/// from an XML file and they are now stored into a ObjectData::ObjectXMLClassData object.
	/// </summary>
	/// <param name="objData">The object in which are stored the properties of the current object.</param>
	void SetObjectProperties(ObjectData::ObjectXMLClassData &objData);
	#pragma endregion
};
