#pragma once

#include <global.hpp>
#include <shader>

struct RectangleData;

class FilledRectangle : public Shader
{
public:
	FilledRectangle();
	void create();
	void render(RectangleData &data, bool picking);
	~FilledRectangle();
};

