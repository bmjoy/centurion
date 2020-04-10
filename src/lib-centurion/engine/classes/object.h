/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>
#include <gl_object_sprite.h>
#include "object-data.h"
#include <game/pass.h>

class Unit;
class Building;
class Decoration;

namespace Pass { class PassGrid; }

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
	/// (???) Da rivedere se l'object e una struttura.
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
	/// <summary>
	/// Da rivedere.
	/// This function marks the current object as selected.
	/// </summary>
	void Select(void);

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
	/// (???) Da cancellare perche un duplicato -> Da cancellare soprattutto nei vari XML.
	/// </summary>
	/// <returns></returns>
	float GetSelectionRadius(void);

	void SetSelectionRadius(const float par_selectionRadius);

	/// <summary>
	/// This function returns the class name of the current object.
	/// </summary>
	/// <returns>The class name of the current object.</returns>
	std::string GetClassName(void) const;

	/// <summary>
	/// This function sets the class name of the current object.
	/// </summary>
	/// <param name="par_className">The class name.</param>
	void SetClassName(const std::string par_className);

	/// <summary>
	/// This function sets the name of the current object that will be shown when a single object is selected.
	/// </summary>
	/// <param name="par_singularName">The singular name of the object.</param>
	void SetSingularName(const std::string par_singularName);

	/// <summary>
	/// This function returns the name of the current object that will be shown when a single object is selected.
	/// </summary>
	/// <returns>The singular name of the object.</returns>
	std::string GetSingularName(void);

	/// <summary>
	/// This function sets the prural of the current object that will be shown when more objects belong to the same class are selected at the same time.
	/// </summary>
	/// <param name="par_pluralName">The prural name.</param>
	void SetPluralName(const std::string par_pluralName);

	/// <summary>
	/// This function returns the prural of the current object that will be shown when more objects belong to the same class are selected at the same time.
	/// </summary>
	/// <returns>The prural name.</returns>
	std::string GetPluralName(void);

	/// <summary>
	/// This function sets the script name of the current object. If the script name is already used, it will be setted using empty string.
	/// </summary>
	/// <param name="_scriptName">The script name of the current object. It must be univocal. </param>
	void SetScriptName(const std::string _scriptName);

	/// <summary>
	/// This function return the univocal script name of the current object.
	/// </summary>
	/// <returns>The script name of the current object; if the current object doesn't have a script name, an empty string will be returned.</returns>
	std::string GetScriptName(void);
	
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
	std::string GetRaceName(void);

	/// <summary>
	/// This function sets the race name of the current object.
	/// </summary>
	/// <param name="par_raceName">The race name.</param>
	void SetRaceName(const std::string par_raceName);

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
	std::string GetType(void);

	/// <summary>
	/// This function sets the type of the current object.
	/// </summary>
	/// <param name="par_type">The type of the current object.</param>
	void SetType(const std::string par_type);

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
	void SetPosition(const glm::vec3 pos);

	/// <summary>
	/// This function returns the position of the current object.
	/// </summary>
	/// <returns></returns>
	glm::vec3 GetPosition(void);

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
	void UpdatePass(void);
	void ClearPass(void);

	void MarkAsMoving(void) { bIsBeingMoved = true; }
	void MarkAsNotMoving(void) { bIsBeingMoved = false; }
	bool IsBeingMoved(void) { return bIsBeingMoved; }
	bool IsPlaceable(void);

	/// <summary>
	/// This function create a specific object.
	/// </summary>
	/// <param name="_className">The class of the object.</param>
	/// <param name="_temporary">Boolean: true = the object is temporary (e.g. an object that is being inserted in the editor)</param>
	/// <returns>Trye if the object has been created; false otherwise.</returns>
	bool Create(const std::string _className, const bool _temporary = false);

	/// <summary>
	/// This abstract function perfoms the rendering of the current object. It must be overread.
	/// </summary>
	virtual void Render(const bool picking, const unsigned int clickID = 0) = 0;

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
	static std::vector<Building*> GetListOfBuildings(void);

	/// <summary>
	/// This function returns a list of all the units of the game.
	/// </summary>
	/// <returns>A list of units.</returns>
	static std::vector<Unit*> GetListOfUnits(void);

	/// <summary>
	/// This function returns a list of all the decoration of the game.
	/// </summary>
	/// <returns>A list of decoration</returns>
	static std::vector<Decoration*> GetListOfDecorations(void);

	#pragma endregion

	/// <summary>
	/// Public constructor.
	/// </summary>
	GObject(void);

	/// <summary>
	/// The destructor.
	/// </summary>
	~GObject(void);

protected:
	#pragma region Protected members
	
	/// /// <summary>
	/// Protected and abstract method that checks if the object is placeable and sets the relative boolean.
	/// </summary>
	virtual void CheckIfPlaceable(void) = 0;
	
	bool bIsPlaceable = false;
	Pass::PassGrid* pass_grid;
	glObjectSprite::SpriteData spriteData;
	Player *player;
	glm::vec3 pickingColor;
	std::map<std::string, std::string> methods;
	std::map<std::string, std::string> sounds;
	bool bIsBeingMoved = false;

	/// <summary>
	/// This function checks if the current object is the selected object (namely, if the user has done left click on it).
	/// </summary>
	/// <param name="par_clickID">The click.</param>
	/// <returns>True if the current object is selected; false otherwise.</returns>
	bool CheckIfSelected(const unsigned int par_clickID);

	/// <summary>
	/// This function marks the current object as selected (true) or not selected (false).
	/// </summary>
	/// <param name="par_selected">If the current objects must be marked as selected or not selected.</param>
	void MarkAsSelected(const bool par_selected);

	#pragma endregion

private:
	unsigned short int playerID;
	unsigned int pickingID;
	unsigned int race;
	unsigned int sight;
	float selectionRadius;
	float radius;
	glm::vec3 position;
	std::string singularName;
	std::string pluralName;
	std::string scriptName;
	std::string raceName;
	std::string className;
	std::string type;
	bool bSelected;
	bool bIsWaterObject = false;
	bool canBeClonedInEditor = false;
	bool bAlwaysVisibleInGameMinimap = false;

	#pragma region Static Properties
	static unsigned int numberOfObjects;
	static unsigned int numberOfBuildings;
	static unsigned int numberOfUnits;
	static unsigned int numberOfDecorations;
	static GObject* GameObjects[MAX_NUMBER_OF_OBJECTS];
	static std::map<const unsigned int, std::string> scriptNamesMap;
	#pragma endregion

	#pragma region Private members
	/// <summary>
	/// This functions sets the properties of the current object when it is being created. 
	/// The values of these properties were previously read 
	/// from an XML file and they are now stored into a ObjectData::ObjectXMLClassData object.
	/// </summary>
	/// <param name="objData">The object in which are stored the properties of the current object.</param>
	/// <param name="_temporary">Boolean: true = the object is temporary (e.g. an object that is being inserted in the editor)</param>
	void SetObjectProperties(ObjectData::ObjectXMLClassData &objData, const bool _temporary = false);
	#pragma endregion
};
