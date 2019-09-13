#pragma once

#include <global.hpp>
#include <shader.h>
#include <stb_image.h>

class Grid : public Shader
{
public:
	Grid();
	void create();
	void render(glm::mat4 projMat, glm::mat4 viewMat);
	~Grid();
};
