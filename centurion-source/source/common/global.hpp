#ifndef GLOBAL_H
#define GLOBAL_H

#include <global>

namespace GLB {
	extern glm::mat4 MENU_PROJECTION;
	extern glm::mat4 CAMERA_PROJECTION;
	extern glm::mat4 MINIMAP_PROJECTION;
	extern bool WINDOW_CLOSE;
	extern bool MAIN_MENU;
	extern bool GAME;
	extern bool EDITOR;
	extern bool MOUSE_LEFT, MOUSE_RIGHT;
	extern bool MOUSE_RELEASE;
	extern int MOUSE_SCROLL;
	extern bool MOUSE_SCROLL_BOOL;
	extern bool UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY, ESC_KEY;
	extern bool WIREFRAME;
	extern float RES_X_RATIO, RES_Y_RATIO;
	extern float Z_NOISE;
	extern bool CTRL_BUTTON;
	extern std::vector<glm::vec3> COLORS;
	extern GLFWwindow *MAIN_WINDOW;
	extern bool RESET;
}

namespace MAP {
	extern float MIN_Z, MAX_Z;
	extern float XYSCALE, ZSCALE, XSEED, YSEED;
}

namespace SHD {
	extern int GRID_SHADER_ID;
}

namespace GAME {
	extern bool MENU_IS_ACTIVE;
	extern int PLAYERS_NUMBER;
	extern int PLAYERS_NUMBER_MAX;
	extern int TOWNHALL_RADIUS;
	extern std::vector<std::string> RACES;
	extern int ZOOM_CURRENT;
	extern float ZOOM_CAMERA_FACTOR;
	extern int MAP_WIDTH, MAP_HEIGHT;
	extern float CAMERA_MOVESPEED;
	extern bool MINIMAP_IS_ACTIVE;
	extern bool GRID_IS_ACTIVE;
}

#endif
