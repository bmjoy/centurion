#pragma once 

#include <gui>
#include <json.hpp>
#include <object>
#include <unit>
#include <building>
#include <decoration>
#include <player>
#include "match.h"

using namespace std;
using namespace glm;

class UIGame;

using namespace unit;
using namespace building;
using namespace decoration;

// global game variables  --> see source/game/game_functions.cpp

extern int playersNumber;
extern int maxPlayersNumber;
extern bool gameMenuStatus;
extern bool gameMinimapStatus;
extern bool gameGridStatus;
extern int mapWidth, mapHeight;
//extern float cameraMovespeed;
extern int currentZoomCamera;
extern float zoomCameraFactor;
extern float townhallRadius;
extern float outpostRadius;
extern bool selRectangleIsActive;
static const float cameraThreshold = 20.f;
extern bool blockMinimap;
extern int selectedUnits;

extern map<int, Building> buildings;
extern map<int, Building*> independent_buildings;
extern map<int, Unit> units;
extern map<int, Decoration> decorations;

extern vector<string> outposts;

extern array<Player, 8> playersList;

//
//	GAME CLASS --> source/game/game.cpp
//

class Strategy : public Match {
public:
	Strategy();
	bool game_is_created() { return gameIsCreated; }
	void reset();
	void create();
	void handleKeyboardControls();
	void run();
	gui::Rectangle selRectangle;
	~Strategy();
private:
	//Surface *surface;
	//UIGame *ui;
	mat4 view, projection;
	bool gameIsCreated;
};

static Strategy mygame;
Strategy *GAME(); // --> definition in game_functions.cpp

// 
// PRERENDERED MINIMAP

class Minimap {
public:
	Minimap();
	void create();
	void render();
	void setStatus(bool b) { isCreated = b; }
	bool getStatus() { return isCreated; }
	~Minimap();
private:
	bool isCreated;
};

static Minimap myminimap;
Minimap *MINIMAP();


//
// GAME FUNCTIONS --> source/game/game_functions.cpp
//

struct SelRectPoints {
	float startX, startY, lastX, lastY;
	float minX, maxX, minY, maxY;
};

static SelRectPoints selRectCoords;
SelRectPoints *SelRectCoords();
static float cameraToX = 0.f, cameraToY = 0.f;
static float cameraLastX = 0.f;
static float cameraLastY = 0.f;

//void renderObjectsPicking();
void tracing();
void clearBuffers();
void goToPosition();
void renderObjects();
void renderSelRectangle();
void setMinimapProjection();
void generateSettlements(vector<vec2> &locs);
void generateOutposts(vector<vec2> &locs);
