#pragma once

#include <global.hpp>
#include <shader.h>

class FilledRectangle : public Shader
{
public:
	FilledRectangle();
	void create();
	void render(glm::mat4 model, glm::vec4 color);
	~FilledRectangle();
};

