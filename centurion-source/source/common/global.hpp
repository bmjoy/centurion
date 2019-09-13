#ifndef GLOBAL_H
#define GLOBAL_H

#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include "../engine/window.h"

namespace GLB {
	extern int WINDOW_WIDTH, WINDOW_HEIGHT;
	extern int WINDOW_WIDTH_ZOOMED, WINDOW_HEIGHT_ZOOMED;
	extern glm::mat4 MENU_PROJECTION;
	extern glm::mat4 CAMERA_PROJECTION;
	extern glm::mat4 MINIMAP_PROJECTION;
	extern bool WINDOW_CLOSE;
	extern bool MAIN_MENU;
	extern bool GAME;
	extern bool MOUSE_LEFT, MOUSE_RIGHT;
	extern bool MOUSE_RELEASE;
	extern int MOUSE_X, MOUSE_Y, MOUSE_Y_2D;
	extern int MOUSE_LEFT_X, MOUSE_LEFT_Y, MOUSE_RIGHT_X, MOUSE_RIGHT_Y, MOUSE_RIGHT_Y_2D;
	extern int MOUSE_SCROLL;
	extern bool MOUSE_SCROLL_BOOL;
	extern bool UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY;
	extern bool WIREFRAME;
	extern float RES_X_RATIO, RES_Y_RATIO;
	extern float Z_NOISE;
	extern bool CTRL_BUTTON;
	extern std::vector<glm::vec3> COLORS;
	extern GLFWwindow *MAIN_WINDOW;
}

namespace MAP {
	extern float MIN_Z, MAX_Z;
	extern float XYSCALE, ZSCALE, XSEED, YSEED;
}

namespace SHD {
	extern int IMAGE_SHADER_ID;
	extern int GRID_SHADER_ID;
	extern int F_RECTANGLE_SHADER_ID;
	extern int USPRITE_SHADER_ID;
	extern int BSPRITE_SHADER_ID;
	extern int FONT_SHADER_ID;
}

namespace GAME {
	extern int PLAYERS_NUMBER, PLAYERS_NUMBER_MAX;
	extern std::vector<float> TOWNHALL_POS;
	extern float TOWNHALL_RADIUS;
	extern std::vector<std::string> RACES;
	extern int ZOOM_CURRENT;
	extern float ZOOM_CAMERA_FACTOR;
	extern float UI_BOTTOM_HEIGHT;
	extern float UI_TOP_HEIGHT;
	extern int MAP_WIDTH, MAP_HEIGHT;
	extern int CAMERA_POS_X, CAMERA_POS_Y;
	extern float CAMERA_MOVESPEED;
	extern bool MINIMAP_IS_ACTIVE;
	extern std::vector<glm::vec3> PLAYERS_COLOR;
	extern std::vector<std::string> PLAYERS_RACE;
	extern bool GRID_IS_ACTIVE;
}

namespace PATH {
	extern int CELL_GRID_SIZE;
	extern int GRID_MATRIX[1000][1500];
	extern int CLOSED_NODES[1000][1500];
	extern int OPEN_NODES[1000][1500];
	extern int DIR_MAP[1000][1500];
}

/* GLOBAL FUNCTIONS */

static void forceGameClosure(std::string reason, std::string caption) {
	MessageBox(NULL, reason.c_str(), caption.c_str(), MB_ICONERROR);
	GLB::WINDOW_CLOSE = true;
}

static std::string ReadFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open()) {
		printf("Failed to read %s! File doesn't exist.", fileLocation);
		return "";
	}
	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

/* function for empty rectangle creation*/
static std::array<float, 8> getCoords(float x0, float y0, float w, float h) {
	float x1 = x0;
	float y1 = y0 - h;

	float x2 = x1 + w;
	float y2 = y1;

	float x3 = x2; 
	float y3 = y0;

	std::array<float, 8> output = { x0, y0, x1, y1, x2, y2, x3, y3 };

	return output;
}

static float Distance(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

static float distEllipse(float x, float y, float cx, float cy, float r) {
	return ((x - cx)*(x - cx) / (r*r) + 1.5f * (y - cy) * (y - cy) / (r * r));
}

static int get_id() {
	unsigned char data[4];
	glReadPixels(GLB::MOUSE_LEFT_X, GLB::MOUSE_LEFT_Y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	int pickedID =
		data[0] +
		data[1] * 256 +
		data[2] * 256 * 256;
	return pickedID;
}

static void updatePassMatrix(std::vector<std::vector<int>> mat, glm::vec2 position){
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < mat[0].size(); j++) {
			if(PATH::GRID_MATRIX[((int)mat.size() - i) + (int)position.y][j + (int)position.x] == 0){
				PATH::GRID_MATRIX[((int)mat.size() - i) + (int)position.y][j + (int)position.x] = mat[i][j];
			}
		}
	}
}

static int getGridInfoFromPoint(float x, float y) {
	return PATH::GRID_MATRIX[(int)x / PATH::CELL_GRID_SIZE][(int)y / PATH::CELL_GRID_SIZE];
}

static glm::vec2 getZoomedCoords(float xCoord, float yCoord) {
	float x, y;
	x = xCoord * (float)GLB::WINDOW_WIDTH_ZOOMED / (float)GLB::WINDOW_WIDTH + (float)GAME::CAMERA_POS_X;
	y = yCoord * (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT + (float)GAME::CAMERA_POS_Y;
	return glm::vec2(x, y);
}

#endif