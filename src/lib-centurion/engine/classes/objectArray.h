/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>

class GObject;
class Building;
class Unit;
class Decoration;

namespace ObjectArray
{
	/// <summary>
	/// This function return the total number of objects that the game has been created.
	/// </summary>
	/// <returns>The total number of objects.</returns>
	unsigned int GetNumberOfObjects(void);

	/// <summary>
	/// This function return the total number of buildings that the game has been created.
	/// </summary>
	/// <returns>The total number of buildings.</returns>
	unsigned int GetNumberOfBuildings(void);

	/// <summary>
	/// This function return the total number of units that the game has been created.
	/// </summary>
	/// <returns>The total number of units.</returns>
	unsigned int GetNumberOfUnits(void);

	/// <summary>
	/// This function return the total number of decorations that the game has been created.
	/// </summary>
	/// <returns>The total number of decorations.</returns>
	unsigned int GetNumberOfDecorations(void);

	/// <summary>
	/// This function adds an object to the game.
	/// </summary>
	/// <param name="index">The index of the object.</param>
	/// <param name="object">A pointer to the object.</param>
	void AddGameObject(const unsigned int index, GObject* object);

	/// <summary>
	/// This function removes an object from the game.
	/// </summary>
	/// <param name="index">The index of the objet that is waiting for its removal.</param>
	void RemoveGameObject(const unsigned int index);

	/// <summary>
	/// This function deletes all the objects from the game and so it even allow to reuse all script names.
	/// </summary>
	void ResetGameObjects(void);

	/// <summary>
	/// This functions returns an existing object by its unique picking ID.
	/// </summary>
	/// <param name="ID">The picking ID of the object.</param>
	/// <returns>A pointer to an object; if the object doesn't exist, nullptr will be returned.</returns>
	GObject* GetObjectByID(const unsigned int ID);

	/// <summary>
	/// This function return a list of all the buildings of the game.
	/// </summary>
	/// <returns>A list of buildings.</returns>
	std::vector<Building*> GetListOfBuildings(void);

	/// <summary>
	/// This function returns a list of all the units of the game.
	/// </summary>
	/// <returns>A list of units.</returns>
	std::vector<Unit*> GetListOfUnits(void);

	/// <summary>
	/// (???) Da rivedere.
	/// This function returns a list of all the indipendent buildings in the game.
	/// </summary>
	/// <returns>A list of buildings.</returns>
	std::vector<Building*> GetListOfIndipendentBuildings(void);

	/// <summary>
	/// This function returns a list of all the decoration of the game.
	/// </summary>
	/// <returns>A list of decoration</returns>
	std::vector<Decoration*> GetListOfDecorations(void);

	bool CheckIfIdNameExists(const std::string name);

	unsigned int GetPickingIdByIdName(const std::string name);

	void RemoveIdName(const std::string name);

	void AssignIdName(const unsigned int pickingid, const std::string name);

	void Render(const bool picking, const unsigned int clickid);
};