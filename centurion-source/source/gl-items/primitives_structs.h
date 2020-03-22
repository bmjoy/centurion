#ifndef PRIMITIVES_STRUCTS_H
#define PRIMITIVES_STRUCTS_H

#include <header_primitives.h>

using namespace std;
using namespace glm;

struct ImageData {
	vec3 pickingColor;
	string imageName;
	float x, y, w, h;
	float texW, texH, ratioX, ratioY;
	int pickingID;
	int origin;
	GLuint textureID;
};

struct RectangleData {
	vec4 borderColor, backColor, pickingColor;
	string type;
	float x, y, w, h;
	int origin;
};

struct CircleData {
	vec4 borderColor, backColor;
	string type;
	float x, y, w, h;
	float borderWidth;
	int origin;
};

struct UnitData {
	map<int, int[2]> spriteSize;
	map<int, int> Frames;
	map<int, int> Durations;
	map<string, int> States;
	vec3 playerColor;
	vec3 pickingColor;
	string className;
	int hitBox[2];
	GLint currentState;
	int currentFrame;
	int currentDir;
	int yOffset;
	int maxDirections;
	GLuint textureID;
};

#endif