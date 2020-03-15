#ifndef BUILDINGSPRITE_H
#define BUILDINGSPRITE_H

#include "shader.h"

class BuildingSprite : public Shader
{
public:
	BuildingSprite();
	BuildingSprite(int shaderID);
	void addPath(string Path) { entPathList.push_back(Path); }
	GLuint getTextureId(string className);
	void create();
	void render(GLuint texID, bool clickable, float x, float y, float w, float h, bool picking, int pickingId, bool selected, vec3 *playerColor, bool not_placeable = false);
	~BuildingSprite();

private:
	vector<string> entPathList;
	string texturePath;       // texture path

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

	int r, g, b;

	void genBuffers();
};

static BuildingSprite bsprite;

#endif