/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>
#include "game.h"

//class UIGame;
//class Player;
//class Building;
//class Unit;
//class Decoration;
class GObject;

namespace Game
{
	namespace Editor
	{
		/// <summary>
		/// This function create the editor.
		/// </summary>
		void Create(void);

		/// <summary>
		/// This function runs the editor and perfoms the rendering of the editor.
		/// </summary>
		void Run(void);

		/// <summary>
		/// (???) Da rivedere. 
		/// This function notices the pressure of a specific key and consequently performs an appropriate action. 
		/// </summary>
		void handleKeyboardControls(void);

		/// <summary>
		/// This function adds an element into the editor object tree.
		/// </summary>
		/// <param name="filter1">The Type of the element (building, decoration, unit).</param>
		/// <param name="filter2">The Category of the element (for instance: outpost, stronghold, trees).</param>
		/// <param name="filter2">The Class of the element (for instance: Etownhall, EArcher).</param>
		void AddEditorObjectTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3);

		/// <summary>
		/// This function returns the list of all the types of objects that can be placed into editor (building, decoration, unit).
		/// </summary>
		/// <returns>The list of all the types of objects that can be placed into editor</returns>
		std::vector<std::string>* GetEditorObjectTreeList1(void);

		/// <summary>
		/// This function returns the list of all the categories of objects that can be placed into editor 
		/// for a specific type (for instance: outpost, stronghold, trees).
		/// </summary>
		/// <param name="filter1">The type of the object (building, decoration, unit).</param>
		/// <returns>The list of all the categories of objects that can be placed into editor for a specific type.</returns>
		std::vector<std::string>* GetEditorObjectTreeList2(const std::string filter1);

		/// <summary>
		/// This function returns the list of all the classes of objects that can be placed into editor 
		/// for a specific type and for a specific category (for instance: Etownhall, EArcher).
		/// </summary>
		/// <param name="filter1">The type of the object (building, decoration, unit).</param>
		/// <param name="filter2">The category of the object (for instance: outpost, stronghold, trees).</param>
		/// <returns>The list of all the classes of objects that can be placed into editor for a specific type and for a specific category. </returns>
		std::vector<std::string>* GetEditorObjectTreeList3(const std::string filter1, const std::string filter2);


		/// <summary>
		/// This function adds an element into the editor terrain tree.
		/// </summary>
		/// <param name="filter1">The Type of the terrain (grass, ground, sand,...).</param>
		/// <param name="filter2">The name the terrain (for instance: GRASS_1, GRASS_2, SAND_1, ...).</param>
		void AddEditorTerrainTreeElement(const std::string& filter1, const std::string& filter2);

		/// <summary>
		/// This function returns the list of all the types of terrains that can be placed in the map (grass, ground, sand...).
		/// </summary>
		/// <returns>The list of all the types of terrains that can be placed in the map</returns>
		std::vector<std::string>* GetEditorTerrainTreeList1(void);

		/// <summary>
		/// This function returns the list of all the terrains that can be placed in the map 
		/// </summary>
		/// <param name="filter1">The type of the terrain (grass, ground, ...).</param>
		/// <returns>The list of all the terrains that can be placed into the map for a specific type.</returns>
		std::vector<std::string>* GetEditorTerrainTreeList2(const std::string filter1);

		/// <summary>
		/// This function adds a specific object, selected from the object menu, into the editor map.
		/// </summary>
		/// <param name="type">The object's type (building, decoration, unit).</param>
		/// <param name="className">The object's class name (for instance: ETownhall, EArcher). </param>
		void InsertingObject(std::string type = "", std::string className = "");

		/// <summary>
		/// This function moves an object in the editor map. 
		/// </summary>
		void ShiftSelectedObject(void);

		/// <summary>
		/// This function creates a random map 
		/// </summary>
		void GenerateRandomMap(const int nPlayers);

		/// <summary>
		/// This function returns a boolean that indicates if an object is being inserted
		/// </summary>
		/// <returns></returns>
		bool IsInsertingObject(void);

		/// <summary>
		/// This function returns a boolean that indicates if an object is being moved
		/// </summary>
		bool IsMovingObject(void);

		/// <summary>
		/// This function changes the type of the terrain using the mouse cursor
		/// </summary>
		void ChangeTerrainType(std::string type = "");

		/// <summary>
		/// This function returns a boolean that indicates if terrain is being changed
		/// </summary>
		bool IsChangingTerrain(void);

		/// <summary>
		/// This method closes and resets the editor, and sets the environment to "menu"
		/// </summary>
		void Close(void);

		/// <summary>
		/// This method ask for closing the editor, opening a message window
		/// </summary>
		void AskForClosing(void);
	};
};

