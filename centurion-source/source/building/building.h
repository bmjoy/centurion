#pragma once

#include <global.hpp>
#include <json.hpp>
#include "../game/game_object.h"
#include "building_sprite.h"

class Building : public GObject
{
public:
	Building();
	void create();
	void create_pass();
	void render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID);
	~Building();

private:
	BSprite building_sprite;
	std::vector<std::vector<int>> building_grid;
	json grid_data;
};
