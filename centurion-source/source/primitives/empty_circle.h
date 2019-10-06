#pragma once

#include <global.hpp>
#include <shader.h>

struct CircleData {
	glm::vec4 borderColor, backColor;
	std::string type;
	float x, y, w, h;
	float borderWidth;
	int origin;
};

class EmptyCircle : public Shader
{
public:
	EmptyCircle();
	void create();
	void render(CircleData &data);
	~EmptyCircle();
};

