#ifndef GRID_H
#define GRID_H

#include "shader.h"

class Grid : public Shader
{
public:
	Grid();
	void create();
	void reset();
	void update();
	void render();
	~Grid();
};

static Grid grid;

#endif