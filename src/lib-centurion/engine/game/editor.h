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


class Editor : public Game
{
public:
	/// <summary>
	/// This function create the editor.
	/// </summary>
	static void Create(void);
	/// <summary>
	/// This function runs the editor and perfoms the rendering of the editor.
	/// </summary>
	static void Run(void);
	/// <summary>
	/// (???) Da rivedere. 
	/// This function notices the pressure of a specific key and consequently performs an appropriate action. 
	/// </summary>
	static void handleKeyboardControls(void);

	/// <summary>
	/// This function adds an element into the editor tree.
	/// </summary>
	/// <param name="filter1">The Type of the element (building, decoration, unit).</param>
	/// <param name="filter2">The Category of the element (for instance: outpost, stronghold, trees).</param>
	/// <param name="filter2">The Class of the element (for instance: Etownhall, EArcher).</param>
	static void AddEditorTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3);

	/// <summary>
	/// This function returns the list of all the types of objects that can be placed into editor (building, decoration, unit).
	/// </summary>
	/// <returns>The list of all the types of objects that can be placed into editor</returns>
	static std::vector<std::string>* GetEditorTreeList1(void);

	/// <summary>
	/// This function returns the list of all the categories of objects that can be placed into editor 
	/// for a specific type (for instance: outpost, stronghold, trees).
	/// </summary>
	/// <param name="filter1">The type of the object (building, decoration, unit).</param>
	/// <returns>The list of all the categories of objects that can be placed into editor for a specific type.</returns>
	static std::vector<std::string>* GetEditorTreeList2(const std::string filter1);

	/// <summary>
	/// This function returns the list of all the classes of objects that can be placed into editor 
	/// for a specific type and for a specific category (for instance: Etownhall, EArcher).
	/// </summary>
	/// <param name="filter1">The type of the object (building, decoration, unit).</param>
	/// <param name="filter2">The category of the object (for instance: outpost, stronghold, trees).</param>
	/// <returns>The list of all the classes of objects that can be placed into editor for a specific type and for a specific category. </returns>
	static std::vector<std::string>* GetEditorTreeList3(const std::string filter1, const std::string filter2);

	/// <summary>
	/// This function adds a specific object, selected from the object menu, into the editor map.
	/// </summary>
	/// <param name="type">The object's type (building, decoration, unit).</param>
	/// <param name="className">The object's class name (for instance: ETownhall, EArcher). </param>
	static void InsertingObject(std::string type = "", std::string className = "");

	/// <summary>
	/// This function returns a boolean that indicates if an object is being inserted
	/// </summary>
	/// <returns></returns>
	static bool IsInsertingObject(void) { return (tmpObject != nullptr); }

	/// <summary>
	/// This method closes and resets the editor, and sets the environment to "menu"
	/// </summary>
	static void Close(void);
	/// <summary>
	/// The destructor.
	/// </summary>
	~Editor(void);
private:
	static std::vector<std::array<std::string, 3>> editorTree;
	static std::vector<std::string> editorTreeList1;
	static std::vector<std::string> editorTreeList2;
	static std::vector<std::string> editorTreeList3;

	static GObject* tmpObject;
	Editor(void);
};
