#pragma once

#include <global.hpp>
#include <json.hpp>
#include "game_functions.h"
#include "../gui/ui_game.h"
#include "../player/player.h"
#include "../engine/cursor_point.h"

class Game
{
public:
	Game();
	void create(std::vector<Player> *ListOfPlayers);
	void run();
	int getSelectedUnits() { return selectedUnits; }
	~Game();

private:

	/* OBJECTS */
	std::vector<Player> *playersList;
	Camera camera;
	Surface surface;
	EmptyRectangle minimapRectangle;
	EmptyRectangle selectionRectangle;
	Building b;
	Unit unit;
	UIGame ui;
	CPoint cursor_point;

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

	std::array<float, 8> minimap_rect_coords, sel_rect_coords;	
};