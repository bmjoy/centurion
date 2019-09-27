#pragma once

#include <global.hpp>
#include <json.hpp>
#include <shader.h>

struct UnitData {
	std::map<int, int[2]> spriteSize;
	std::map<int, int> Frames;
	std::map<int, int> Durations;
	std::map<std::string, int> States;
	glm::vec3 playerColor;
	glm::vec3 pickingColor;
	std::string className;
	int hitBox[2];
	GLint currentState;
	int currentFrame;
	int currentDir;
	int yOffset;
	int maxDirections;
	GLuint textureID;
};

class UnitSprite : public Shader
{
public:
	UnitSprite();
	UnitSprite(int shaderID);
	void addPath(std::string Path) { entPathList.push_back(Path); }
	void create();
	void getTextureInfo(UnitData *uData);
	void render(UnitData d, glm::vec3 &pos, bool picking);
	~UnitSprite();

private:
	std::vector<std::string> entPathList;
	std::string texturePath;       // texture path

	std::map<std::string, int[2]> spriteSize;

	unsigned char *data;
	unsigned int indices[6] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	GLfloat vertices[20] = {
		// positions				// uv coords		
		-0.5f,		-0.5f,		0.0f,		0.0f, 1.0,   // in basso a sx
		0.5f,		-0.5f,		0.0f,		1.0, 1.0,    // in basso a dx
		0.5f,		0.5f,		0.0f,		1.0, 0.0f,   // in alto a dx
		-0.5f,		0.5f,		0.0f,		0.0f, 0.0f   // in alto a sx
	};

	void genBuffers();
};

