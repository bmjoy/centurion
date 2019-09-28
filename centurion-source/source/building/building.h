#pragma once

#include <global.hpp>
#include <json.hpp>
#include <objects.h>
#include "../game/game_object.h"

class Unit;

class Building : public GObject
{
public:
	Building();
	void create();
	void render(bool picking, int clickID);
	int UnitsInBuilding();
	std::vector<Unit> UnitsInHolder();
	~Building();

private:
	std::vector<std::vector<int>> building_grid;
	std::vector<Unit> unitsInside;
	int w, h, nrChannels;
	GLuint textureID;
	bool clickableInMinimap;
	//sound selectionSound; TODO
};
