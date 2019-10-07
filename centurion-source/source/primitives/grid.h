#pragma once

#include <global.hpp>
#include <shader>
#include <stb_image.h>
#include <mapgen>

class Grid : public Shader
{
public:
	Grid();
	void create();
	void update();
	void render();
	~Grid();
};
