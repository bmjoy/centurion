#ifndef CURSORIMAGE_H
#define CURSORIMAGE_H

#include "shader.h"

class CursorImage : public Shader
{
public:
	CursorImage();
	void create();
	void render(float x, float y, string &currentState);
	~CursorImage();
private:
	float w, h;
	/* texture variables */
	string initPath;
	array<string, 3> texturePaths;
	array<string, 3> textureNames;
};

static CursorImage cursor;

#endif