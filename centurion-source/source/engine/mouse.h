#pragma once

#include <global.hpp>
#include <map_generation.h>
#include <shader.h>

class Mouse : public Shader
{
public:
	Mouse();
	void create();
	void render();
	void mouse_control(int lastX, int lastY);
	~Mouse();

private:
	float w, h;

	float znoise;
	float yzoomed;

	glm::vec3 position;
	glm::mat4 modelMat;

	std::string currentState;

	/* texture variables */
	std::string initPath;
	std::array<std::string, 3> texturePaths;
	std::array<std::string, 3> textureNames;
	
};

