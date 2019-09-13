#pragma once

#include <global.hpp>
#include <json.hpp>
#include <map_generation.hpp> //For 3D coordinates
#include "../game/game_object.h"
#include "unit_sprite.h"
#include "../primitives/image.h" //Just for debugging purpose
#include "../primitives/filled_rectangle.h"//Just for debugging purpose
#include "../pathfinding/a_star.h"


class Unit : public GObject
{
public:
	Unit();
	void create();
	void select(bool b);
	void set_position(float x, float y);
	void render(glm::mat4 viewMat);
	~Unit();

private:
	USprite sprite;
	Image circlePos2D;
	Image circlePos3D;

	FilledRectangle rectanglePath;
	aStar Path;
	std::vector<glm::ivec2> path;

	std::string currentState;

	glm::mat4 model;

	/* Position */
	glm::vec3 position2D;
	glm::vec3 position3D;

	int dir;
	int frames_counter;

	void set_frame();
	void set_direction();

	void position_update();

	void walk_behaviour();
	glm::vec2 to_point;
	bool is_Moving = false;
	float start_x, start_y;
	float distance, res_distance;
	float angle;

	void znoise_update();
	float znoise;
	glm::vec3 values;

	float xPosGrid, yPosGrid;

	void pathfinding();

	json entityData;
};