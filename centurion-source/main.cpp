#include <global.hpp>
#include <json.hpp>

#include "source/engine/engine.h"


// things to do: 
// 1) minimap prerender

namespace GLB {
	int WINDOW_WIDTH = 1024; // 2560
	int WINDOW_HEIGHT = 768; // 1440
	int WINDOW_WIDTH_ZOOMED;
	int WINDOW_HEIGHT_ZOOMED;
	glm::mat4 MENU_PROJECTION;
	glm::mat4 CAMERA_PROJECTION;
	glm::mat4 MINIMAP_PROJECTION;
	bool WINDOW_CLOSE = false;
	bool MAIN_MENU = true;
	bool GAME = false;
	bool MOUSE_LEFT = false, MOUSE_RIGHT = false;
	bool MOUSE_RELEASE = false;
	int MOUSE_X = 0, MOUSE_Y = 0, MOUSE_Y_2D = 0;
	int MOUSE_LEFT_X = 0, MOUSE_LEFT_Y = 0, MOUSE_RIGHT_X = 0, MOUSE_RIGHT_Y = 0, MOUSE_RIGHT_Y_2D = 0;
	int MOUSE_SCROLL = 0;
	bool MOUSE_SCROLL_BOOL = false;
	bool UP_KEY = false, DOWN_KEY = false, LEFT_KEY = false, RIGHT_KEY = false;
	bool WIREFRAME = false;
	float RES_X_RATIO;
	float RES_Y_RATIO;
	float Z_NOISE = 0.0f;
	bool CTRL_BUTTON = false;
	std::vector<glm::vec3> COLORS;
	GLFWwindow *MAIN_WINDOW;
	bool DEBUG = false;
	std::array<float, 8> SELECTION_RECTANGLE_COORDS;
}

namespace MAP {
	float MIN_Z = 0.0f, MAX_Z = 0.0f;
	float XYSCALE = 80.0f, ZSCALE = 0.025/3.0, XSEED = 0.0, YSEED = 0.0f;
}

namespace SHD {
	int IMAGE_SHADER_ID = 0;
	int GRID_SHADER_ID = 0;
	int F_RECTANGLE_SHADER_ID = 0;
	int USPRITE_SHADER_ID = 0;
	int BSPRITE_SHADER_ID = 0;
	int FONT_SHADER_ID = 0;
}

namespace GAME {
	int MAP_WIDTH = 30000, MAP_HEIGHT = 20000;
	int PLAYERS_NUMBER = 1;
	int PLAYERS_NUMBER_MAX = 10;
	float TOWNHALL_RADIUS = 1875.0f;
	int ZOOM_CURRENT = 8;
	float ZOOM_CAMERA_FACTOR = 100.0f;
	float UI_BOTTOM_HEIGHT = 60.0f;
	float UI_TOP_HEIGHT = 100.0f;
	int CAMERA_POS_X = 0, CAMERA_POS_Y = 0;
	float CAMERA_MOVESPEED;
	bool MINIMAP_IS_ACTIVE = false;
	std::vector<float> TOWNHALL_POS;
	std::vector<std::string> RACES;
	std::vector<glm::vec3> PLAYERS_COLOR;
	std::vector<std::string> PLAYERS_RACE;
	bool GRID_IS_ACTIVE = false;
}

namespace PATH {
	int CELL_GRID_SIZE = 20;
	int GRID_MATRIX[1000][1500] = { 0 };
	int GRID_MATRIX_2D[1000][1500] = { 0 };
	int CLOSED_NODES[1000][1500] = { 0 };
	int OPEN_NODES[1000][1500] = { 0 };
	int DIR_MAP[1000][1500] = { 0 };
}



int main() {
	std::ifstream settings_path("settings.json");
	json settings = json::parse(settings_path);

	GLB::DEBUG = (bool)settings["debug"].get<int>();
	GLB::WINDOW_WIDTH = settings["window_width"];
	GLB::WINDOW_HEIGHT = settings["window_height"];
	GLB::WINDOW_WIDTH_ZOOMED = GLB::WINDOW_WIDTH;
	GLB::WINDOW_HEIGHT_ZOOMED = GLB::WINDOW_HEIGHT;
	GLB::MENU_PROJECTION = glm::ortho(0.0f, (float)GLB::WINDOW_WIDTH, 0.0f, (float)GLB::WINDOW_HEIGHT, -100.0f, 100.0f);
	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, (float)GLB::WINDOW_WIDTH_ZOOMED, 0.0f, (float)GLB::WINDOW_HEIGHT_ZOOMED, -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
	GLB::RES_X_RATIO = (float)GLB::WINDOW_WIDTH / (float)GAME::MAP_WIDTH;
	GLB::RES_Y_RATIO = (float)GLB::WINDOW_HEIGHT / (float)GAME::MAP_HEIGHT;
	GAME::CAMERA_MOVESPEED = (float)settings["camera_movespeed"];

	std::ifstream data_path("assets/data/data.json");
	//Close the game if it wasn't able to find or process data.json file
	if (!data_path.good()) {
		forceGameClosure("Error code 0x00000001\n\nThe game is unable to find or process DATA file.\nForced application shutdown has started.", "Imper4m");
	}
	json data = json::parse(data_path);

	for (int i = 0; i < data["races"].size(); i++) {
		GAME::RACES.push_back(data["races"][i]);
	}

	int r, g, b;
	for (int i = 0; i < data["player_colors"].size(); i++) {
		r = data["player_colors"][i]["r"];
		g = data["player_colors"][i]["g"];
		b = data["player_colors"][i]["b"];
		GLB::COLORS.push_back(glm::vec3(r, g, b));
	}

	Engine engine = Engine();

	return engine.launch();
}