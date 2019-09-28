#pragma once

#include <global.hpp>
#include <shader.h>

#include <json.hpp>

class BuildingSprite : public Shader
{
public:
	BuildingSprite();
	BuildingSprite(int shaderID);
	void addPath(std::string Path) { entPathList.push_back(Path); }
	GLuint getTextureId(std::string className);
	void create();
	void render(GLuint texID, bool clickable, float x, float y, float w, float h, bool picking, int pickingId, bool selected, glm::vec3 *playerColor);
	~BuildingSprite();

private:

	std::vector<std::string> entPathList;
	std::string texturePath;       // texture path

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

