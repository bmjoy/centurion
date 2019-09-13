#include "stb_image.h" //Necessary for texture manipulation
#include "unit.h"

Unit::Unit() {
	dir = 0;
	frames_counter = 0.0;

	currentState = "idle";
	position2D = glm::vec3(100.0f, 100.0f, 0.0f);
	position3D = glm::vec3(100.0f, 100.0f, 0.0f);

	Path = aStar();
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
	//position_update();
	//walk_behaviour();
	//znoise_update();

	if (GLB::MOUSE_RIGHT) pathfinding();

	sprite.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	sprite.apply_view_matrix(viewMat);

	circlePos2D.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	circlePos2D.apply_view_matrix(viewMat);
	circlePos3D.apply_projection_matrix(GLB::CAMERA_PROJECTION);
	circlePos3D.apply_view_matrix(viewMat);

	GAME::MINIMAP_IS_ACTIVE ? rectanglePath.apply_projection_matrix(GLB::MINIMAP_PROJECTION) : rectanglePath.apply_projection_matrix(GLB::CAMERA_PROJECTION);

	rectanglePath.apply_view_matrix(viewMat);

	model = glm::translate(glm::mat4(1.0f), position2D);

	if (!GAME::MINIMAP_IS_ACTIVE) {
		circlePos2D.render(position2D.x, position2D.y);
		circlePos3D.render(position3D.x, position3D.y);

		sprite.render(model, currentState);
	}

	for (int i = 0; i < path.size(); i++) {
		rectanglePath.render(path[i].x * 20.f, path[i].y * 20.f);
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
	if (is_Moving) {
		position2D.x += (to_point.x - start_x) / distance * data["movement_speed"];
		position2D.y += (to_point.y - start_y) / distance * data["movement_speed"];
		res_distance = sqrt(float((to_point.x - position2D.x) * (to_point.x - position2D.x) + (to_point.y - position2D.y) * (to_point.y - position2D.y)));
		if (res_distance < data["movement_speed"]) {
			is_Moving = false;
			currentState = "idle";
		}
	}
}

void Unit::walk_behaviour() {
	if (GLB::MOUSE_RIGHT) {

		to_point = getZoomedCoords((float)GLB::MOUSE_RIGHT_X, (float)GLB::MOUSE_RIGHT_Y_2D);
		start_x = position2D.x;
		start_y = position2D.y;

		distance = sqrt(float((to_point.x - position2D.x) * (to_point.x - position2D.x) + (to_point.y - position2D.y) * (to_point.y - position2D.y)));
		res_distance = distance;
		angle = atan2(to_point.y - start_y, to_point.x - start_x) * 180 / 3.14159265;
		if (angle < 0) { angle += 360.0f; }
		dir = round(angle / 360 * entityData["sprites"][currentState]["directions"]);

		if (distance > data["movement_speed"]) {
			is_Moving = true;
			currentState = "walk";
		}
	}
}

void Unit::pathfinding() {

	GLB::MOUSE_RIGHT = false;

	to_point = getZoomedCoords((float)GLB::MOUSE_RIGHT_X, (float)GLB::MOUSE_RIGHT_Y_2D);
	start_x = position2D.x;
	start_y = position2D.y;

	int jStart = (int)start_x / 20;
	int iStart = (int)start_y / 20;
	int jEnd = (int)to_point.x / 20;
	int iEnd = (int)to_point.y / 20;

	std::cout << "Start: " << iStart << "," << jStart << std::endl;
	std::cout << "Finish: " << iEnd << "," << jEnd << std::endl;

	clock_t start = clock();


	//fix pathfinding click to 1
	while (PATH::GRID_MATRIX[iEnd][jEnd] != 0) {
		iEnd--;
		jEnd--;
	}


	path = Path.pathFind(Location(iStart, jStart), Location(iEnd, jEnd));

	clock_t end = clock();
	double time = double(end - start);
	std::cout << "Time (ms): " << time << std::endl;
}

void Unit::znoise_update() {
	znoise = generateNoise(position2D).zNoise;

	position3D.x = position2D.x;
	position3D.y = position2D.y + znoise;
	sprite.set_znoise(znoise);
}

Unit::~Unit() {

}