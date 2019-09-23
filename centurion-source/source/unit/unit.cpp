#include "stb_image.h" //Necessary for texture manipulation
#include "unit.h"

Unit::Unit() {
	dir = 0;
	frame = 0.0;
	currentState = "idle";
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
	std::ifstream path(data["ent_path"].get<std::string>());
	entityData = json::parse(path);
	sprite = USprite(SHD::USPRITE_SHADER_ID);
	sprite.create(entityData);

	//Show circle position under the unit (Debug only)
	circlePos = Image(SHD::IMAGE_SHADER_ID);
	circlePos.create("assets/ui/mouse/cursor_point.png", "center");

	//Show selection hitbox around the unit (Debug only)
	hitbox.rectangle = EmptyRectangle();
	hitbox.rectangle.compile();
	hitbox.rectangle.init();

	rectanglePath = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	rectanglePath.set_color(glm::vec4(255.f, 0.f, 0.f, 255.f));
	rectanglePath.create(20.f, 20.f, "bottom-left");

	creationTime = glfwGetTime();
}

void Unit::render(glm::mat4 &proj, glm::mat4 &view, bool picking, int clickID) {
	clickSelection = (picking_id == clickID);
	if(GLB::MOUSE_LEFT)	rectangleSelection = unit::isInSelectionRect(hitbox.coords);

	selected = (clickSelection + rectangleSelection > 0);

	if(!picking){
		unit::updateDirection(&sprite, dir);
		unit::updateFrame(&sprite, &creationTime, &frame, entityData["sprites"][currentState]["frames"], entityData["sprites"][currentState]["duration"]);
		unit::updateZ(position2D, &position3D);
		position_update();
		walk_behaviour();		
	}

	sprite.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	sprite.apply_view_matrix(view);

	model = glm::translate(glm::mat4(1.0f), position3D);
	sprite.render(model, currentState, picking, picking_id, &(*player).getPlayerColor());

	if (!GLB::DEBUG) {
		hitbox.rectangle.apply_projection_matrix(GLB::CAMERA_PROJECTION);
		hitbox.coords = getCoords(position3D.x - entityData["hitbox"][0], position3D.y + entityData["hitbox"][1] + entityData["yOffset"], entityData["hitbox"][0] * 2, entityData["hitbox"][1] * 2);
		hitbox.rectangle.create(hitbox.coords);
		hitbox.rectangle.render(view, glm::mat4(1.0f), selected ? glm::vec4(255.0f, 255.0f, 255.0f, 1.0f) : glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	/* debug pathfinding and coordinates */

	if (GLB::DEBUG) {
		circlePos.apply_projection_matrix(GLB::CAMERA_PROJECTION);
		circlePos.apply_view_matrix(view);

		GAME::MINIMAP_IS_ACTIVE ? rectanglePath.apply_projection_matrix(GLB::MINIMAP_PROJECTION) : rectanglePath.apply_projection_matrix(GLB::CAMERA_PROJECTION);
		rectanglePath.apply_view_matrix(view);

		for (int i = 0; i < path.size(); i++) {
			rectanglePath.render(path[i].x, path[i].y);
		}
		if (!GAME::MINIMAP_IS_ACTIVE) {
			circlePos.render(position2D.x, position2D.y);
			circlePos.render(position3D.x, position3D.y);

			hitbox.rectangle.apply_projection_matrix(GLB::CAMERA_PROJECTION);
			hitbox.coords = getCoords(position3D.x - entityData["hitbox"][0], position3D.y + entityData["hitbox"][1] + entityData["yOffset"], entityData["hitbox"][0] * 2, entityData["hitbox"][1] * 2);
			hitbox.rectangle.create(hitbox.coords);
			hitbox.rectangle.render(view, glm::mat4(1.0f), selected ? glm::vec4(255.0f, 0.0f, 255.0f, 1.0f) : glm::vec4(255.0f, 242.0f, 0.0f, 1.0f));
		}
	}
}

void Unit::position_update() {
	if (is_Moving && pathCount < path.size() - 2) {
		currentState = "walk";
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
				dir = round(angle / 360 * entityData["sprites"][currentState]["directions"]);
			}
		}
	}
	if (pathCount == path.size() - 2) {
		pathCount = 0;
		is_Moving = false;
		currentState = "idle";
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

		// set first distance
		distance = unit::getDistance(path, pathCount);
		res_distance = distance;
		
		// set initial direction
		angle = unit::getAngle(path, pathCount);
		dir = round(angle / 360 * entityData["sprites"][currentState]["directions"]);

	}
}

Unit::~Unit() {

}