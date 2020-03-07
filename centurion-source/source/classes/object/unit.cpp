#include "unit.h"
#include <math>
#include <game/strategy.h>
#include <game/game.h>
#include <stb_image.h>
#include <player>
#include <pathfinding>
#include <engine/mouse.h>
#include <surface>


using namespace math;
using namespace glb;



int Unit::unitsCounter = 0;

Unit::Unit() {
	unitData = UnitData();
	unitData.currentDir = 0;
	unitData.currentFrame = 0;

	currentStateStr = "idle";
	position2D = glm::vec3(0.0f, 0.0f, 0.0f);
	position3D = glm::vec3(0.0f, 0.0f, 0.0f);
	pathCount = 0;
	is_Moving = false;
	creationTime = 0.f;
	clickSelection = false;
	rectangleSelection = false;
}

void Unit::set_position(float x, float y) {
	position2D.x = x;
	position2D.y = y;
	position3D.x = x;
	position3D.y = y;
}

void Unit::create() {

	/* store data from entity json */
	std::string state;
	std::ifstream path(data["ent_path"].get<std::string>());
	entityData = json::parse(path);

	unitData.maxDirections = entityData["directions"];
	for (int i = 0; i < entityData["spriteList"].size(); i++) {
		state = entityData["spriteList"][i].get<std::string>();
		unitData.States[state] = i;
		unitData.Frames[i] = entityData["sprites"][state]["frames"].get<int>();
		unitData.Durations[i] = entityData["sprites"][state]["duration"].get<int>();
	}
	unitData.hitBox[0] = entityData["hitbox"][0].get<int>();
	unitData.hitBox[1] = entityData["hitbox"][1].get<int>();
	unitData.yOffset = entityData["yOffset"].get<int>();
	unitData.playerColor = *(player->getPlayerColor());
	unitData.pickingColor = pickingColor;
	unitData.className = className;
	obj::USprite()->getTextureInfo(&unitData);

	selectionCircle = gui::Circle();
	selectionCircle.create("border", 0.f, 0.f, 35.f, 23.f, 8.f, "center");

	// hitbox 
	hitbox.rectangle = gui::Rectangle();
	hitbox.rectangle.create("border", 0.0f, 0.0f, (float)unitData.hitBox[0], (float)unitData.hitBox[1], "center", 0);

	//Show circle position under the unit (Debug only)
	circlePos = gui::Image("circle_pos");
	circlePos.create("center", 0, 0, 0, 0, 0);

	creationTime = (float)glfwGetTime();
}

void Unit::render(bool picking, int clickID, bool not_placeable) {

	clickSelection = (picking_id == clickID);
	if (Game::SelectionRectangle::IsActive()) rectangleSelection = Game::SelectionRectangle::IsInRectangle(hitbox.coords);
	selected = (clickSelection + rectangleSelection > 0);

	hitbox.coords = get_rectangle_coords(position3D.x - unitData.hitBox[0] / 2.f, position3D.y + unitData.hitBox[1] / 2.f + unitData.yOffset, (float)unitData.hitBox[0], (float)unitData.hitBox[1]);

	if (!picking) {
		updateFrame(&creationTime, &unitData.currentFrame, unitData.Frames[unitData.currentState], unitData.Durations[unitData.currentState]);
		updateZ(position2D, &position3D);
		position_update();
		walk_behaviour();
	}

	if (Game::Minimap::IsActive() == false) {
		if (selected) {
			selectionCircle.render(glm::vec4(255.f, 255.f, 255.f, 0.8f), position3D.x, position3D.y);
		}
		obj::USprite()->render(unitData, position3D, picking);

	}

	/* debug pathfinding and coordinates */

	if (Settings::DebugIsActive && !picking) {

		// **** Rectangle Path **** //	
		for (int i = 0; i < pathQuadsList.size(); i++) {
			pathQuadsList[i].render(glm::vec4(255.f, 0.f, 0.f, 255.f));
		}
		// ************************ //

		if (Game::Minimap::IsActive() == false) {
			circlePos.render(false, position2D.x, position2D.y);
			hitbox.rectangle.render(
				selected ? glm::vec4(255.0f, 0.0f, 255.0f, 1.0f) : glm::vec4(255.0f, 242.0f, 0.0f, 1.0f),
				0,
				position3D.x,
				position3D.y + unitData.yOffset,
				0,
				0
			);
		}
	}
}

void Unit::position_update() {
	if (is_Moving && pathCount < path.size() - 2) {
		currentStateStr = "walk";
		unitData.currentState = unitData.States[currentStateStr];

		position2D.x += (path[pathCount + 1].x - path[pathCount].x) / distance * data["movement_speed"];
		position2D.y += (path[pathCount + 1].y - path[pathCount].y) / distance * data["movement_speed"];

		delta = getResDistance(path, pathCount, position2D) - res_distance;
		res_distance = getResDistance(path, pathCount, position2D);

		if (res_distance < 20 || delta > 0) {

			pathCount += 1;

			// update distance
			distance = getDistance(path, pathCount);
			res_distance = distance;

			// update direction
			if (pathCount < path.size() - 2) {
				angle = getAngle(path, pathCount);
				unitData.currentDir = (int)round(angle / 360 * unitData.maxDirections);
			}
		}
	}
	if (pathCount == path.size() - 2) {
		pathCount = 0;
		is_Moving = false;
		currentStateStr = "idle";
		unitData.currentState = unitData.States[currentStateStr];
		pathQuadsList = { };
	}
}

void Unit::walk_behaviour() {
	if (engine::Mouse::RightClick && selected) {

		is_Moving = true;

		startPoint = glm::vec2((int)position2D.x / astar::cellGridSize * astar::cellGridSize, (int)position2D.y / astar::cellGridSize * astar::cellGridSize);
		endPoint = getZoomedCoords(engine::Mouse::GetXRightClick(), engine::Mouse::GetY2DRightClick());

		// pathfinding
		path = getPath(startPoint, endPoint);

		pathQuadsList = { };
		for (int i = 0; i < path.size(); i++) {
			gui::Rectangle tempRect = gui::Rectangle();
			tempRect.create("filled", (float)path[i].x, (float)path[i].y, 20.f, 20.f, "bottom-left", 0);
			pathQuadsList.push_back(tempRect);
		}

		// set first distance
		distance = getDistance(path, pathCount);
		res_distance = distance;

		// set initial direction
		angle = getAngle(path, pathCount);
		unitData.currentDir = (int)round(angle / 360 * unitData.maxDirections);

	}
}

Unit::~Unit() {}

void updateZ(vec3 &pos2d, vec3 *pos3d) {
	float zNoise = mapgen::getNoiseEstimate(pos2d.x, pos2d.y);
	(*pos3d).x = pos2d.x;
	(*pos3d).y = pos2d.y + zNoise;
}

void updateFrame(float *creationTime, int *frame, int max_frames, int duration) {
	if (glfwGetTime() - *creationTime >= duration / 100.f) {
		(*frame)++;
		if (*frame == max_frames - 1) *frame = 0;
		(*creationTime) += duration / 100.f;
	}
}

float getAngle(vector<ivec2> &path, int &n) {
	float angle = (float)atan2(path[n + 1].y - path[n].y, path[n + 1].x - path[n].x) * 180.0f / 3.14159265f;
	if (angle < 0) angle += 360.0f;
	return angle;
}

float getDistance(vector<ivec2> &path, int &n) {
	float deltaX = (float)path[n + 1].x - (float)path[n].x;
	float deltaY = (float)path[n + 1].y - (float)path[n].y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

float getResDistance(vector<ivec2> &path, int &n, vec3 &pos2d) {
	float deltaX = (float)path[n + 1].x - (int)pos2d.x / astar::cellGridSize * astar::cellGridSize;
	float deltaY = (float)path[n + 1].y - (int)pos2d.y / astar::cellGridSize * astar::cellGridSize;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

vector<ivec2> getPath(vec2 &start, vec2 &end) {
	int jStart = (int)start.x / astar::cellGridSize;
	int iStart = (int)start.y / astar::cellGridSize;
	int jEnd = (int)end.x / astar::cellGridSize;
	int iEnd = (int)end.y / astar::cellGridSize;

	//fix pathfinding click to 1
	while (astar::GridMatrix2D()[iEnd * astar::gridWidth + jEnd] != 0) {
		iEnd--;
		jEnd--;
	}

	return astar::pathFind(astar::Location(iStart, jStart), astar::Location(iEnd, jEnd));
}