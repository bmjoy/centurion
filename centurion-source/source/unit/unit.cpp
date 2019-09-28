#include "stb_image.h" //Necessary for texture manipulation
#include "../building/building.h"
#include "unit.h"

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

	// hitbox 
	hitbox.rectangle = gui::Rectangle();
	hitbox.rectangle.create("border", 0, 0, unitData.hitBox[0], unitData.hitBox[1], "center", 0);

	//Show circle position under the unit (Debug only)
	circlePos = gui::Image("circle_pos");
	circlePos.create("center", 0, 0, 0, 0, 0);

	creationTime = glfwGetTime();
}

void Unit::render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID) {

	clickSelection = (picking_id == clickID);
	if(GLB::MOUSE_LEFT)	rectangleSelection = unit::isInSelectionRect(hitbox.coords);

	selected = (clickSelection + rectangleSelection > 0);

	if(!picking){
		unit::updateFrame(&creationTime, &unitData.currentFrame, unitData.Frames[unitData.currentState], unitData.Durations[unitData.currentState]);
		unit::updateZ(position2D, &position3D);
		position_update();
		walk_behaviour();		
	}

	obj::USprite()->render(unitData, position3D, picking);

	if (!GLB::DEBUG && !picking) {
		hitbox.coords = getCoords(position3D.x - unitData.hitBox[0] / 2.f, position3D.y + unitData.hitBox[1] / 2.f + unitData.yOffset, unitData.hitBox[0], unitData.hitBox[1]);
		hitbox.rectangle.render(
			selected ? glm::vec4(255.0f, 255.0f, 255.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
			0,
			position3D.x,
			position3D.y + unitData.yOffset,
			0,
			0
		);
	}

	/* debug pathfinding and coordinates */

	if (GLB::DEBUG && !picking) {

		// **** Rectangle Path **** //
		
		for (int i = 0; i < pathQuadsList.size(); i++) {
			pathQuadsList[i].render(glm::vec4(255.f, 0.f, 0.f, 255.f));
		}
		// ************************ //

		if (!GAME::MINIMAP_IS_ACTIVE) {
			circlePos.render(false, position2D.x, position2D.y);
			circlePos.render(false, position3D.x, position3D.y);

			hitbox.coords = getCoords(position3D.x - unitData.hitBox[0] / 2.f, position3D.y + unitData.hitBox[1] / 2.f + unitData.yOffset, unitData.hitBox[0], unitData.hitBox[1]);
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

		delta = unit::getResDistance(path, pathCount, position2D) - res_distance;
		res_distance = unit::getResDistance(path, pathCount, position2D);

		if (res_distance < 20 || delta > 0) {	

			pathCount += 1;
			
			// update distance
			distance = unit::getDistance(path, pathCount);
			res_distance = distance;

			// update direction
			if (pathCount < path.size() - 2) {
				angle = unit::getAngle(path, pathCount);
				unitData.currentDir = round(angle / 360 * unitData.maxDirections);
			}
		}
	}
	if (pathCount == path.size() - 2) {
		pathCount = 0;
		is_Moving = false;
		currentStateStr = "idle";
		unitData.currentState = unitData.States[currentStateStr];
	}
}

void Unit::walk_behaviour() {
	if (GLB::MOUSE_RIGHT && selected) {

		//GLB::MOUSE_RIGHT = false;
		is_Moving = true;

		startPoint = glm::vec2((int)position2D.x / PATH::CELL_GRID_SIZE * PATH::CELL_GRID_SIZE, (int)position2D.y / PATH::CELL_GRID_SIZE * PATH::CELL_GRID_SIZE);
		endPoint = getZoomedCoords((float)GLB::MOUSE_RIGHT_X, (float)GLB::MOUSE_RIGHT_Y_2D);

		// pathfinding
		path = unit::getPath(startPoint, endPoint);

		pathQuadsList = { };
		for (int i = 0; i < path.size(); i++) {
			gui::Rectangle tempRect = gui::Rectangle();
			tempRect.create("filled", path[i].x, path[i].y, 20.f, 20.f, "bottom-left", 0);
			pathQuadsList.push_back(tempRect);
		}

		// set first distance
		distance = unit::getDistance(path, pathCount);
		res_distance = distance;
		
		// set initial direction
		angle = unit::getAngle(path, pathCount);
		unitData.currentDir = round(angle / 360 * unitData.maxDirections);

	}
}

Unit::~Unit() {

}