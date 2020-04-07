#include "grid.h"
#include <game/pass.h>

using namespace std;
using namespace glm;

Grid::Grid()
{
	vPath = "assets/shaders/terrain/vertex_grid.glsl";
	fPath = "assets/shaders/terrain/fragment_grid.glsl";
	textureID = 0;
}

void Grid::create() {
	glUseProgram(shaderId);

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float vertices[] = {
		// X,Y,Z												// uv coords		
		0.f, 0.f, 0.f,											0.f, 1.f,  // in basso a sx
		(float)MEDIUM_MAP_WIDTH, 0.f, 0.f,						1.f, 1.f,  // in basso a dx
		(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_HEIGHT, 0.f,	1.f, 0.f,  // in alto a dx
		0.f, (float)MEDIUM_MAP_HEIGHT, 0.f,						0.f, 0.f   // in alto a sx
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Pass::GetGridSizeX(), Pass::GetGridSizeY(), 0, GL_RED, GL_UNSIGNED_BYTE, (GLvoid*)Pass::GetGridPtr());
}

void Grid::update() {
	glUseProgram(shaderId);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, Pass::GetGridSizeX(), Pass::GetGridSizeY(), 0, GL_RED, GL_UNSIGNED_BYTE, (GLvoid*)Pass::GetGridPtr());
}

void Grid::render() {

	glUseProgram(shaderId);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glUniform1i(glGetUniformLocation(shaderId, "grid"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}



Grid::~Grid()
{
}
