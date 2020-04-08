/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include "object.h"
#include <ui.h>
#include <gl_unit_sprite.h>

class Building;
class Player;

/// <summary>
/// (???) Classe da rivedere quasi completamente!
/// </summary>
class Unit : public GObject
{
public:
	Unit();
	static int GetCounter() { return unitsCounter; }
	static void IncreaseCounter() { unitsCounter++; }
	static void ResetCounter() { unitsCounter = 0; }
	void SetPosition(const float x, const float y);
	void Render(const bool picking, const unsigned int clickID = 0) override;
	~Unit();
private:
	glUnitSprite::UnitData unitData;

	static int unitsCounter;

	/* unit properties & data */
	std::string currentStateStr;
	float creationTime;

	/* unit position */
	glm::vec3 position2D, position3D;
	bool clickSelection, rectangleSelection;

	/* unit circle if selected */
	gui::Circle selectionCircle;

	/* unit movement & pathfinding */
	bool is_Moving;
	int pathCount;
	float angle, delta, distance, res_distance;
	std::vector<glm::ivec2> path;
	glm::vec2 startPoint, endPoint;
	void position_update();
	void walk_behaviour();

	/* debug objects */
	std::vector<gui::Rectangle> pathQuadsList;
	struct HitBox {
		gui::Rectangle rectangle;
		std::array<float, 8> coords;
	} hitbox;
	gui::Image circlePos;
};


// UNIT FUNCTIONS --> source/unit/functions.cpp

void updateZ(glm::vec3 &pos2d, glm::vec3 *pos3d);
void updateFrame(float *creationTime, int *frame, int max_frames, int duration);
float getAngle(std::vector<glm::ivec2> &path, int &n);
float getDistance(std::vector<glm::ivec2> &path, int &n);
float getResDistance(std::vector<glm::ivec2> &path, int &n, glm::vec3 &pos2d);
std::vector<glm::ivec2> getPath(glm::vec2 &start, glm::vec2 &end);
