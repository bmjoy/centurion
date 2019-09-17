#pragma once

#include <global.hpp>
#include <json.hpp>
#include <map_generation.hpp> //For 3D coordinates
#include "../game/game_object.h"
#include "unit_sprite.h"
#include "../primitives/image.h" //Just for debugging purpose
#include "../primitives/filled_rectangle.h" //Just for debugging purpose
#include "../primitives/empty_rectangle.h" //Just for debugging purpose
#include "../pathfinding/a_star.h"


class Unit : public GObject
{
public:
	Unit();
	void create();
	void select(bool b);
	void set_position(float x, float y);
	void render(glm::mat4 viewMat);
	bool isSelected;
	~Unit();

private:
	USprite sprite;

	/* unit properties & data */
	std::string currentState;
	json entityData;

	/* unit position */
	float znoise;
	glm::vec3 position2D;
	glm::vec3 position3D;
	void znoise_update();

	/* unit movement & pathfinding */
	bool is_Moving;
	int dir;
	int frames_counter;
	int pathCount;
	float angle;
	float delta;
	float distance, res_distance;
	float getResDistance();
	std::vector<glm::ivec2> path;
	glm::vec2 startPoint, endPoint;
	void set_frame();
	void set_direction();
	void position_update();
	void walk_behaviour();
	std::vector<glm::ivec2> pathfinding(glm::vec2 start, glm::vec2 end);
	
	/* unit selection */
	void checkSelRect();
	bool isInSelRect();
	
	/* unit rendering */
	glm::mat4 model;

	/* debug objects */
	FilledRectangle rectanglePath;
	struct HitBox {
		EmptyRectangle rectangle;
		std::array<float, 8> coords;
	} hitbox;
	Image circlePos2D;
	Image circlePos3D;
};