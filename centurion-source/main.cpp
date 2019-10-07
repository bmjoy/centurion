#include <global.hpp>
#include <json.hpp>
#include <engine>

// things to do: 
// 1) minimap prerender

namespace GLB {
	glm::mat4 MENU_PROJECTION;
	glm::mat4 CAMERA_PROJECTION;
	glm::mat4 MINIMAP_PROJECTION;
	bool WINDOW_CLOSE = false;
	bool MAIN_MENU = true;
	bool GAME = false;
	bool EDITOR = false;
	bool MOUSE_LEFT = false, MOUSE_RIGHT = false;
	bool MOUSE_RELEASE = false;
	int MOUSE_SCROLL = 0;
	bool MOUSE_SCROLL_BOOL = false;
	bool UP_KEY = false, DOWN_KEY = false, LEFT_KEY = false, RIGHT_KEY = false, ESC_KEY = false;
	bool WIREFRAME = false;
	float RES_X_RATIO;
	float RES_Y_RATIO;
	float Z_NOISE = 0.0f;
	bool CTRL_BUTTON = false;
	std::vector<glm::vec3> COLORS;
	GLFWwindow *MAIN_WINDOW;
	bool RESET = false;
}

namespace MAP {
	float MIN_Z = 0.0f, MAX_Z = 0.0f;
	float XYSCALE = 80.0f, ZSCALE = 0.025f/3.0f, XSEED = 0.0f, YSEED = 0.0f;
}

namespace SHD {
	int GRID_SHADER_ID = 0;
}

namespace GAME {
	bool MENU_IS_ACTIVE = false;
	int MAP_WIDTH = 30000, MAP_HEIGHT = 20000;
	int PLAYERS_NUMBER = 1;
	int PLAYERS_NUMBER_MAX = 10;
	int TOWNHALL_RADIUS = 1875;
	int ZOOM_CURRENT = 8;
	float ZOOM_CAMERA_FACTOR = 100.0f;
	float CAMERA_MOVESPEED;
	bool MINIMAP_IS_ACTIVE = false;
	std::vector<std::string> RACES;
	bool GRID_IS_ACTIVE = false;
}

/*
** --------------------------------------------------
** --------------------------------------------------
*/


int main() {

	using namespace glb;
	using namespace std;

	//Close the game if it wasn't able to find or process errorCodes.json file
	ifstream errorCodes_path("assets/data/errorCodes.json");
	if (!errorCodes_path.good()) {
		forceGameClosure("  Error code: " + getErrorCode("NOT_FOUND") + string("\n\n  Unable to find or process ERROR CODES file.\n  Forced application shutdown has started."));
	}
	json errorCodes = json::parse(errorCodes_path);
	map<string, string> errorsMap = errorCodes.get<map<string, string>>();
	setErrors(errorsMap);

	ifstream settings_path("settings.json");
	if (!settings_path.good()) {
		//The correct format should be:
		//forceGameClosure("  " + errorsJson["NOT_FOUND"] + "\n\n  " + errorText[TRANSLATE];
		forceGameClosure("  Error code: "  + getErrorCode("NOT_FOUND") + string("\n\n  Unable to find or process SETTINGS file.\n  Forced application shutdown has started."));
	}
	json settings = json::parse(settings_path);

	setBoolean("debug", (bool)settings["debug"].get<int>());
	setParam("window-width", (float)settings["window_width"]);
	setParam("window-height", (float)settings["window_height"]);
	setParam("window-ratio", getParam("window-width") / getParam("window-height"));
	setParam("window-width-zoomed", getParam("window-width") + (GAME::ZOOM_CURRENT - 1) * GAME::ZOOM_CAMERA_FACTOR);
	setParam("window-height-zoomed", getParam("window-height") + (GAME::ZOOM_CURRENT - 1) * GAME::ZOOM_CAMERA_FACTOR / getParam("window-ratio"));

	setParam("ui-bottom-height", 60.f);
	setParam("ui-top-height", 100.f);

	GLB::MENU_PROJECTION = glm::ortho(0.0f, getParam("window-width"), 0.0f, getParam("window-height"), -100.0f, 100.0f);
	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
	GLB::RES_X_RATIO = getParam("window-width") / (float)GAME::MAP_WIDTH;
	GLB::RES_Y_RATIO = getParam("window-height") / (float)GAME::MAP_HEIGHT;
	GAME::CAMERA_MOVESPEED = (float)settings["camera_movespeed"];

	ifstream data_path("assets/data/data.json");
	//Close the game if it wasn't able to find or process data.json file
	if (!data_path.good()) {
		forceGameClosure("  Error code:" + getErrorCode("NOT_FOUND") + "\n\n  Unable to find or process DATA file.\nForced application shutdown has started.");
	}
	json data = json::parse(data_path);

	for (int i = 0; i < data["races"].size(); i++) {
		GAME::RACES.push_back(data["races"][i]);
	}

	for (int i = 0; i < data["player_colors"].size(); i++) {
		GLB::COLORS.push_back(glm::vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]));
	}

	Engine engine = Engine();

	return engine.launch();
}