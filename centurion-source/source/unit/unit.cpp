#include "stb_image.h" //Necessary for texture manipulation
#include "unit.h"

Unit::Unit() {
	dir = 0;
	frames_counter = 0.0;

	currentState = "idle";
	position2D = glm::vec3(100.0f, 100.0f, 0.0f);
	position3D = glm::vec3(100.0f, 100.0f, 0.0f);

	Path = aStar();
	pathCount = 0;
}

void Unit::set_position(float x, float y) {
	position2D.x = x;
	position2D.y = y;
	position3D.x = x;
	position3D.y = y;
}

void Unit::create() {
	//sprite.create(states, pathList, frameList, directions);
	std::ifstream path(data["ent_path"].get<std::string>());
	entityData = json::parse(path);
	sprite = USprite(SHD::USPRITE_SHADER_ID);
	sprite.create(entityData, player_color);

	//Show circle position under unit (Debug only)
	circlePos2D = Image(SHD::IMAGE_SHADER_ID);
	circlePos2D.create("assets/ui/mouse/cursor_point.png", "center");
	circlePos3D = Image(SHD::IMAGE_SHADER_ID);
	circlePos3D.create("assets/ui/mouse/cursor_point.png", "center");


	rectanglePath = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	rectanglePath.set_color(glm::vec4(255.f, 0.f, 0.f, 255.f));
	rectanglePath.create(20.f, 20.f, "bottom-left");
}

void Unit::select(bool b)
{
}

void Unit::render(glm::mat4 viewMat) {
	set_direction();
	set_frame();
	position_update();
	walk_behaviour();
	znoise_update();

	sprite.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	sprite.apply_view_matrix(viewMat);

	circlePos2D.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	circlePos2D.apply_view_matrix(viewMat);
	circlePos3D.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	circlePos3D.apply_view_matrix(viewMat);

	GAME::MINIMAP_IS_ACTIVE ? rectanglePath.apply_projection_matrix(GLB::MINIMAP_PROJECTION) : rectanglePath.apply_projection_matrix(GLB::CAMERA_PROJECTION);

	rectanglePath.apply_view_matrix(viewMat);

	model = glm::translate(glm::mat4(1.0f), position3D);

	if (!GAME::MINIMAP_IS_ACTIVE) {
		sprite.render(model, currentState);
	}

	/* debug pathfinding and coordinates */

	if (GLB::DEBUG) {
		for (int i = 0; i < path.size(); i++) {
			rectanglePath.render(path[i].x, path[i].y);
		}
		if (!GAME::MINIMAP_IS_ACTIVE) {
			circlePos2D.render(position2D.x, position2D.y);
			circlePos3D.render(position3D.x, position3D.y);
		}
	}

}

void Unit::set_direction() {
	sprite.set_direction(dir);
}

void Unit::set_frame() {
	if ((frames_counter % entityData["sprites"][currentState]["duration"]) == 0) {
		sprite.set_frame(frames_counter / entityData["sprites"][currentState]["duration"], entityData["sprites"][currentState]["frames"]);
		if (frames_counter == entityData["sprites"][currentState]["duration"] * (entityData["sprites"][currentState]["frames"] - 1)) {
			frames_counter = 0;
		}
	}
	frames_counter++;
}

void Unit::position_update() {
	if (is_Moving && pathCount < path.size() - 2) {

		currentState = "walk";


		position2D.x += (path[pathCount + 1].x - path[pathCount].x) / distance * data["movement_speed"];
		position2D.y += (path[pathCount + 1].y - path[pathCount].y) / distance * data["movement_speed"];

		//std::cout << "Actual distance: " << getResDistance() << "; Previous distance: " << res_distance << "\n";

		delta = getResDistance() - res_distance;

		res_distance = getResDistance();

		if (res_distance < 20 || delta > 0) {	

			pathCount += 1;
			
			// update distance
			distance = sqrt(float((path[pathCount + 1].x - path[pathCount].x) * (path[pathCount + 1].x - path[pathCount].x) + (path[pathCount + 1].y - path[pathCount].y) * (path[pathCount + 1].y - path[pathCount].y)));
			res_distance = distance;

			// update direction
			if (pathCount < path.size() - 2) {
				angle = atan2(path[pathCount + 1].y - path[pathCount].y, path[pathCount + 1].x - path[pathCount].x) * 180 / 3.14159265;
				if (angle < 0) { angle += 360.0f; }
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

float Unit::getResDistance(){
	return sqrt(float((path[pathCount + 1].x - (int)position2D.x / 20 * 20) * (path[pathCount + 1].x - (int)position2D.x / 20 * 20) + (path[pathCount + 1].y - (int)position2D.y / 20 * 20) * (path[pathCount + 1].y - (int)position2D.y / 20 * 20)));
}

void Unit::walk_behaviour() {
	if (GLB::MOUSE_RIGHT) {

		GLB::MOUSE_RIGHT = false;
		is_Moving = true;

		startPoint = glm::vec2((int)position2D.x / 20 * 20, (int)position2D.y / 20 * 20);
		endPoint = getZoomedCoords((float)GLB::MOUSE_RIGHT_X, (float)GLB::MOUSE_RIGHT_Y_2D);

		// pathfinding
		path = pathfinding(startPoint, endPoint);

		// set first distance
		distance = sqrt(float((path[pathCount+1].x - path[pathCount].x) * (path[pathCount + 1].x - path[pathCount].x) + (path[pathCount + 1].y - path[pathCount].y) * (path[pathCount + 1].y - path[pathCount].y)));
		res_distance = distance;
		
		// set initial direction
		angle = atan2(path[pathCount + 1].y - path[pathCount].y, path[pathCount + 1].x - path[pathCount].x) * 180 / 3.14159265;
		if (angle < 0) { angle += 360.0f; }
		dir = round(angle / 360 * entityData["sprites"][currentState]["directions"]);

	}
}



std::vector<glm::ivec2> Unit::pathfinding(glm::vec2 start, glm::vec2 end) {

	int jStart = (int)start.x / 20;
	int iStart = (int)start.y / 20;
	int jEnd = (int)end.x / 20;
	int iEnd = (int)end.y / 20;

	//std::cout << "Start: " << iStart << "," << jStart << std::endl;
	//std::cout << "Finish: " << iEnd << "," << jEnd << std::endl;

	clock_t startTime = clock();

	//fix pathfinding click to 1
	while (PATH::GRID_MATRIX_2D[iEnd][jEnd] != 0) {
		iEnd--;
		jEnd--;
	}

	std::vector<glm::ivec2> thePath = Path.pathFind(Location(iStart, jStart), Location(iEnd, jEnd));

	//clock_t endTime = clock();
	//double time = double(endTime - startTime);
	//std::cout << "Time (ms): " << time << std::endl;

	

	return thePath;
}

void Unit::znoise_update() {
	znoise = generateNoise(position2D).zNoise;

	position3D.x = position2D.x;
	position3D.y = position2D.y + znoise - PATH::CELL_GRID_SIZE/2;
}

Unit::~Unit() {

}