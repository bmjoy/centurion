#pragma once

#include <global.hpp>
#include <shader.h>

class FilledRectangle : public Shader
{
public:
	FilledRectangle();
	FilledRectangle(int shaderID);
	void set_color(glm::vec4 col);
	void create(float width, float height, std::string origin);
	void render(float xPos, float yPos, bool picking = false, int picking_id = 0);
	~FilledRectangle();

private:
	float w, h;
	float x0, y0, x1, y1, x2, y2, x3, y3;
	int r, g, b;
	glm::mat4 modelMat;
	glm::vec4 color;

};

