#pragma once

#include <global.hpp>
#include <shader>

class CursorImage : public Shader
{
public:
	CursorImage();
	void create();
	void render(float x, float y, std::string &currentState);
	~CursorImage();
private:
	float w, h;
	/* texture variables */
	std::string initPath;
	std::array<std::string, 3> texturePaths;
	std::array<std::string, 3> textureNames;
};

