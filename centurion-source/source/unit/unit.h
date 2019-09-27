#pragma once

#include <json.hpp>
#include <objects.h>

#include "../game/game_object.h"
#include "unit_functions.h"
#include "../gui/rectangle.h"
#include "../gui/image.h"


class Unit : public GObject
{
public:
	Unit();
	void create();
	void set_position(float x, float y);
	void render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID);
	~Unit();

private:

	UnitData unitData;

	/* unit properties & data */
	std::string currentStateStr;
	json entityData;
	float creationTime;

	/* unit position */
	glm::vec3 position2D;
	glm::vec3 position3D;
	bool clickSelection;
	bool rectangleSelection;

	/* unit movement & pathfinding */
	bool is_Moving;
	int pathCount;
	float angle;
	float delta;
	float distance, res_distance;
	std::vector<glm::ivec2> path;
	glm::vec2 startPoint, endPoint;
	void position_update();
	void walk_behaviour();
	
	/* unit rendering */

	/* debug objects */
	std::vector<gui::Rectangle> pathQuadsList;
	struct HitBox {
		std::array<float, 8> coords;
	} hitbox;
	gui::Image circlePos;
};