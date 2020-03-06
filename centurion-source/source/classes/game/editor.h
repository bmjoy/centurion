#pragma once

#include <gui>
#include <json.hpp>
#include <object>
#include <unit>
#include <building>
#include <decoration>
#include "game.h"

using namespace std;
using namespace glm;

class UIGame;
class Player;

//
//	EDITOR CLASS --> source/editor/editor.cpp
//

class Editor : public Game
{
public:
	static void reset() { isCreated = false; }
	static void Create();
	static void Run();
	static void handleKeyboardControls();
	~Editor();
private:
	Editor();
};

// 
//	EDITOR FUNCTIONS
//

namespace editor {
	using namespace unit;
	using namespace building;
	using namespace decoration;

	extern Unit unitTemp;
	extern Building buildingTemp;
	extern Decoration decorTemp;
	extern bool movingObject;
	static float movingObjectXPos, movingObjectYPos, movingObjectStartXMouse, movingObjectStartYMouse;
	static bool movingObjectRestore;

	void prepareObject(string type, string classname);
	void insertingObject(string type, string classname);
	void addObject(string type);
	void changeTerrain(int terrainType);
	void clearEditorVariables();
	bool areWindowsClosed();
	void moveObjects();
};



