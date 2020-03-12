#ifndef FILLEDRECTANGLE_H
#define FILLEDRECTANGLE_H

#include "shader.h"

class FilledRectangle : public Shader
{
public:
	FilledRectangle();
	void create();
	void render(RectangleData &data, bool picking);
	~FilledRectangle();
};

static FilledRectangle fRect;

#endif