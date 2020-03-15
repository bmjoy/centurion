#ifndef DECORATIONSPRITE_H
#define DECORATIONSPRITE_H

#include "shader.h"
#include "primitives_structs.h"

class DecorationSprite : public Shader
{
public:
	DecorationSprite();
	void addPath(string Path) { entPathList.push_back(Path); }
	GLuint getTextureID(string classname);
	void create();
	void render(DecorationData d, bool not_placeable = false);
	~DecorationSprite();

private:
	vector<string> entPathList;
	map<string, int[2]> spriteSize;

	unsigned char *data;
	unsigned int indices[6] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	GLfloat vertices[20] = {
		// positions				// uv coords		
		-0.5f,		-0.5f,		0.0f,		0.0f, 1.0,   // in basso a sx
		0.5f,		-0.5f,		0.0f,		1.0, 1.0,    // in basso a dx
		0.5f,		0.5f,		0.0f,		1.0, 0.0f,   // in alto a dx
		-0.5f,		0.5f,		0.0f,		0.0f, 0.0f   // in alto a sx
	};

	void genBuffers();
};

static DecorationSprite dsprite;

#endif