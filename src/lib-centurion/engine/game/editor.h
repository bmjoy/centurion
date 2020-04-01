/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>
#include <classes/object.h>
#include "game.h"

//class UIGame;
//class Player;
//class Building;
//class Unit;
//class Decoration;


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
	/// <param name="element">The element.</param>
	static void AddEditorTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3);

	static std::vector<std::string>* GetEditorTreeList1(void);
	static std::vector<std::string>* GetEditorTreeList2(const std::string filter1);
	static std::vector<std::string>* GetEditorTreeList3(void);

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
	Editor(void);
};

#pragma region Editor functions 

namespace editor 
{
	//extern Unit unitTemp;
	//extern Building* buildingTemp;
	//extern Decoration* decorTemp;
	//extern bool movingObject;
	//static float movingObjectXPos, movingObjectYPos, movingObjectStartXMouse, movingObjectStartYMouse;
	//static bool movingObjectRestore;

	/// <summary>
	/// (???) Le tre funzioni di seguito sono da rivedere/cancellare.
	/// </summary>
	void prepareObject(const std::string type, const std::string classname);
	void insertingObject(const std::string type, const std::string classname);
	void addObject(const std::string type);
	/// <summary>
	/// This function changes the terrain.
	/// </summary>
	void changeTerrain(const int terrainType);
	/// <summary>
	/// This function checks if the windows of the editor are closed or not.
	/// </summary>
	/// <returns>True if the windows are closed; false otherwise</returns>
	bool areWindowsClosed(void);
	/// <summary>
	/// (???) Da rivedere.
	/// This function allows the movement of an object in the editor.
	/// </summary>
	void moveObjects(void);
};

#pragma endregion
