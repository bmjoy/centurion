#pragma once

#include <global.hpp>
#include <json.hpp>
#include "../player/player.h"

class GObject
{

public:

	GObject();
	void set_position(glm::vec3 pos);
	void set_class(std::string class_name);
	void set_player(Player *p);
	void set_id(int pickingId);
	bool getSelected() { return selected; }
	int get_xPos() { return (int)position.x; }
	int get_yPos() { return (int)position.y; }
	~GObject();

protected:
	Player *player;
	glm::vec3 pickingColor;
	bool selected;
	json data;
	glm::vec3 position;
	std::string className;
	int picking_id;
};

