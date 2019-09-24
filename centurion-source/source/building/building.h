#pragma once

#include <global.hpp>
#include <json.hpp>
#include <objects.h>
#include "../game/game_object.h"


class Building : public GObject
{
public:
	Building();
	void create_pass();
	void render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID);
	~Building();

private:
	//BSprite building_sprite;
	std::vector<std::vector<int>> building_grid;
	json grid_data;
};
