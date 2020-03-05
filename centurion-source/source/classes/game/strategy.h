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
extern bool gameGridStatus;
extern int mapWidth, mapHeight;
extern int currentZoomCamera;
extern float zoomCameraFactor;
extern float townhallRadius;
extern float outpostRadius;
static const float cameraThreshold = 20.f;
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
	static Strategy GetInstance();
	bool game_is_created() { return gameIsCreated; }
	void handleKeyboardControls();
	void reset();
	void Create() override;
	void Run() override;
	
	~Strategy();
private:
	Strategy();
	static Strategy* strategy;
	mat4 view, projection;
	bool gameIsCreated;
};

//
// GAME FUNCTIONS --> source/game/game_functions.cpp
//


static float cameraToX = 0, cameraToY = 0;
//static SelRectPoints selRectCoords;
//SelRectPoints *SelRectCoords();


//void renderObjectsPicking();
void tracing();
void clearBuffers();
void goToPosition();
//void renderObjects();
//void renderSelRectangle();
void setMinimapProjection();
void generateSettlements(vector<vec2> &locs);
void generateOutposts(vector<vec2> &locs);
