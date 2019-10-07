#ifndef GLOBAL_H
#define GLOBAL_H

#include <global.h>
#include "../engine/window.h"

struct SelRectPoints {
	float minX;
	float maxX;
	float minY;
	float maxY;
};

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
	extern int MOUSE_X, MOUSE_Y, MOUSE_Y_2D;
	extern int MOUSE_LEFT_X, MOUSE_LEFT_Y, MOUSE_RIGHT_X, MOUSE_RIGHT_Y, MOUSE_RIGHT_Y_2D;
	extern int MOUSE_SCROLL;
	extern bool MOUSE_SCROLL_BOOL;
	extern bool UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY, ESC_KEY;
	extern bool WIREFRAME;
	extern float RES_X_RATIO, RES_Y_RATIO;
	extern float Z_NOISE;
	extern bool CTRL_BUTTON;
	extern std::vector<glm::vec3> COLORS;
	extern GLFWwindow *MAIN_WINDOW;
	extern SelRectPoints SEL_RECT_COORDS; //Only a temporar way to test multiple selection
	extern bool GAME_CLEAR;
	extern LPCSTR GAME_NAME;
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
	extern float UI_BOTTOM_HEIGHT;
	extern float UI_TOP_HEIGHT;
	extern int MAP_WIDTH, MAP_HEIGHT;
	extern int CAMERA_POS_X, CAMERA_POS_Y;
	extern float CAMERA_MOVESPEED;
	extern bool MINIMAP_IS_ACTIVE;
	extern bool GRID_IS_ACTIVE;
}

/* GLOBAL FUNCTIONS */

static void forceGameClosure(std::string reason) {
	MessageBox(NULL, reason.c_str(), GLB::GAME_NAME, MB_ICONERROR);
	GLB::WINDOW_CLOSE = true;
}

static void showGameWarning(std::string reason) {
	MessageBox(NULL, reason.c_str(), GLB::GAME_NAME, MB_ICONINFORMATION);
}

static std::string ReadFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);
	if (!fileStream.is_open()) {
		printf("DEBUG: Failed to read %s! File doesn't exist.", fileLocation);
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

static glm::vec2 getZoomedCoords(float xCoord, float yCoord) {
	float x, y;
	x = xCoord * glb::getParam("window-width-zoomed") / glb::getParam("window-width") + (float)GAME::CAMERA_POS_X;
	y = yCoord * glb::getParam("window-height-zoomed") / glb::getParam("window-height") + (float)GAME::CAMERA_POS_Y;
	return glm::vec2(x, y);
}

static float getYMinimapCoord(float x) {
	return glb::getParam("window-height") * (x - GAME::UI_BOTTOM_HEIGHT) / (glb::getParam("window-height") - GAME::UI_BOTTOM_HEIGHT - GAME::UI_TOP_HEIGHT);
}

static bool cursorInGameScreen() { 
	return (GLB::MOUSE_LEFT_Y > GAME::UI_BOTTOM_HEIGHT) && (GLB::MOUSE_LEFT_Y < (glb::getParam("window-height") - GAME::UI_TOP_HEIGHT));
}

static std::vector<std::string> get_all_files_names_within_folder(std::string folder){
	std::vector<std::string> names;
	std::string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

static void clearAndSwapBuffers(GLFWwindow *window) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

static glm::vec3 getPickingColorFromID(int pickingID) {
	int r = (pickingID & 0x000000FF) >> 0;
	int g = (pickingID & 0x0000FF00) >> 8;
	int b = (pickingID & 0x00FF0000) >> 16;
	return glm::vec3(r, g, b);
}

#endif
