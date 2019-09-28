#pragma once

#include <global.hpp>
#include <json.hpp>
#include "game_functions.h"
#include "../interface/game_ui.h"
#include "../player/player.h"
#include "../engine/cursor_point.h"

class Game
{
public:
	Game();
	bool game_is_created() { return gameIsCreated; }
	void reset() { gameIsCreated = false; }
	void create(std::vector<Player> *ListOfPlayers);
	void run();
	void clear();
	int getSelectedUnits() { return selectedUnits; }
	~Game();

private:

	/* OBJECTS */
	std::vector<Player> *playersList;
	Camera camera;
	Surface *surface;
	Building b;
	Unit unit;
	UIGame ui;
	CPoint cursor_point;
	gui::Rectangle selRectangle;

	std::map<int, Building> buildingList;
	std::map<int, Unit> unitList;

	double lastTime;
	float threshold, cameraLastX, cameraLastY;
	int objectId, click_id, selectedUnits;
	bool blockMinimap;
	
	json settl_data;
	std::string r;
	glm::vec2 origin;
	glm::mat4 view, projection;
	bool gameIsCreated;
};