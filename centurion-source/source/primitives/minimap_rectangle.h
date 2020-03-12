#ifndef MINIMAPRECTANGLE_H
#define MINIMAPRECTANGLE_H

#include "shader.h"

class MinimapRectangle : public Shader
{
public:
	MinimapRectangle();
	void create();
	void update();
	void render();
	~MinimapRectangle();
};

static MinimapRectangle mmRect;

#endif