#pragma once

#include <global.hpp>
#include <json.hpp>

class GObject
{

public:

	GObject();
	void set_position(glm::vec3 pos);
	void set_class(std::string className);
	void set_player(int playerId);
	void set_id(int pickingId);
	int get_xPos() { return (int)position.x; }
	int get_yPos() { return (int)position.y; }
	int picking_id;
	~GObject();

protected:

	glm::vec3 player_color;
	json data;
	glm::vec3 position;
};

