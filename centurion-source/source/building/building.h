#pragma once

#include <global.hpp>
#include <json.hpp>
#include <objects.h>
#include "../game/game_object.h"


class Building : public GObject
{
public:
	Building();
	void create();
	void render(bool picking, int clickID);
	~Building();

private:
	std::vector<std::vector<int>> building_grid;
	int w, h, nrChannels;
	glm::mat4 model;
	GLuint textureID;
	bool clickableInMinimap;
};
