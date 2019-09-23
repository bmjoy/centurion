#pragma once

#include <global.hpp>
#include <shader.h>

#include <json.hpp>

class BSprite : public Shader
{
public:
	BSprite();
	BSprite(int shaderID);
	void create(json ent_data);
	void render(float x, float y, bool picking, int pickingId, bool selected, glm::vec3 *playerColor);
	~BSprite();

private:

	std::string path;       // texture path
	
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


	/* matrix transformations */
	glm::mat4 modelMat;

	int r, g, b;

	void genBuffers();
};

