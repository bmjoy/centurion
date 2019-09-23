#pragma once

#include <json.hpp>
#include "../game/game_object.h"

#include "unit_functions.h"


#include "../primitives/image.h" //Just for debugging purpose
#include "../primitives/filled_rectangle.h" //Just for debugging purpose
#include "../primitives/empty_rectangle.h" //Just for debugging purpose



class Unit : public GObject
{
public:
	Unit();
	void create();
	void set_position(float x, float y);
	void render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID);
	~Unit();

private:
	USprite sprite;

	/* unit properties & data */
	std::string currentState;
	json entityData;
	float creationTime;

	/* unit position */
	glm::vec3 position2D;
	glm::vec3 position3D;
	bool clickSelection;
	bool rectangleSelection;

	/* unit movement & pathfinding */
	bool is_Moving;
	int dir;
	int frame;
	int pathCount;
	float angle;
	float delta;
	float distance, res_distance;
	std::vector<glm::ivec2> path;
	glm::vec2 startPoint, endPoint;
	void position_update();
	void walk_behaviour();
	
	/* unit rendering */
	glm::mat4 model;

	/* debug objects */
	FilledRectangle rectanglePath;
	struct HitBox {
		EmptyRectangle rectangle;
		std::array<float, 8> coords;
	} hitbox;
	Image circlePos;
};