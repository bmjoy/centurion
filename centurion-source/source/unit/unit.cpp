#include "stb_image.h" //Necessary for texture manipulation
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

	//Show circle position under the unit (Debug only)
	circlePos = Image(SHD::IMAGE_SHADER_ID);
	circlePos.create("assets/ui/mouse/cursor_point.png", "center");

	creationTime = glfwGetTime();
}

void Unit::render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID) {

	clickSelection = (picking_id == clickID);
	if(GLB::MOUSE_LEFT)	rectangleSelection = unit::isInSelectionRect(hitbox.coords);

	if (GLB::MOUSE_LEFT) std::cout << clickID << "\n";

	selected = (clickSelection + rectangleSelection > 0);

	if(!picking){
		unit::updateFrame(&creationTime, &unitData.currentFrame, unitData.Frames[unitData.currentState], unitData.Durations[unitData.currentState]);
		unit::updateZ(position2D, &position3D);
		position_update();
		walk_behaviour();		
	}

	obj::USprite()->render(unitData, position3D, picking);

	if (!GLB::DEBUG && !picking) {
		hitbox.coords = getCoords(position3D.x - entityData["hitbox"][0], position3D.y + entityData["hitbox"][1] + entityData["yOffset"], entityData["hitbox"][0] * 2, entityData["hitbox"][1] * 2);
		obj::ERectangle()->apply_projection_matrix(GLB::CAMERA_PROJECTION);		
		obj::ERectangle()->create(hitbox.coords);
		obj::ERectangle()->render(view, glm::mat4(1.0f), selected ? glm::vec4(255.0f, 255.0f, 255.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	/* debug pathfinding and coordinates */

	if (GLB::DEBUG && !picking) {
		circlePos.apply_projection_matrix(GLB::CAMERA_PROJECTION);
		circlePos.apply_view_matrix(view);

		// **** Rectangle Path **** //
		
		GAME::MINIMAP_IS_ACTIVE ? obj::FRectangle()->apply_projection_matrix(GLB::MINIMAP_PROJECTION) : obj::FRectangle()->apply_projection_matrix(GLB::CAMERA_PROJECTION);	
		obj::FRectangle()->apply_view_matrix(view);

		for (int i = 0; i < pathQuadsList.size(); i++) {
			pathQuadsList[i].render();
		}
		// ************************ //

		if (!GAME::MINIMAP_IS_ACTIVE) {
			circlePos.render(position2D.x, position2D.y);
			circlePos.render(position3D.x, position3D.y);

			obj::ERectangle()->apply_projection_matrix(GLB::CAMERA_PROJECTION);
			hitbox.coords = getCoords(position3D.x - entityData["hitbox"][0], position3D.y + entityData["hitbox"][1] + entityData["yOffset"], entityData["hitbox"][0] * 2, entityData["hitbox"][1] * 2);
			obj::ERectangle()->create(hitbox.coords);
			obj::ERectangle()->render(view, glm::mat4(1.0f), selected ? glm::vec4(255.0f, 0.0f, 255.0f, 1.0f) : glm::vec4(255.0f, 242.0f, 0.0f, 1.0f));
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
			tempRect.set_color(glm::vec4(255.f, 0.f, 0.f, 255.f));
			tempRect.create("filled", path[i].x, path[i].y, 20.f, 20.f, "bottom-left");
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