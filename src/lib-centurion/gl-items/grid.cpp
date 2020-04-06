#include "grid.h"

#include <game/strategy.h>
#include <mapgen/mapgen.h>
#include <pathfinding/pathfinding.h>

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

	for (int y = 0; y < GRIDSIZE_Y; ++y) {
		for (int x = 0; x < GRIDSIZE_X; ++x) {
			gridData.push_back(0);
			gridData.push_back(0);
			gridData.push_back(0);
			gridData.push_back(0);
		}
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GRIDSIZE_X, GRIDSIZE_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&gridData[0]);
}

void Grid::SetGridDataCell(const unsigned int idx, const unsigned int val)
{
	if (val == 1)
	{
		gridData[idx * 4 + 0] = 0;
		gridData[idx * 4 + 1] = 0;
		gridData[idx * 4 + 2] = 0;
		gridData[idx * 4 + 3] = 100;
	}
	else
	{
		gridData[idx * 4 + 0] = 255;
		gridData[idx * 4 + 1] = 255;
		gridData[idx * 4 + 2] = 255;
		gridData[idx * 4 + 3] = 100;
	}
}

void Grid::reset() {
	glUseProgram(shaderId);
	//unsigned char* gridData = new unsigned char[GRIDSIZE_X * GRIDSIZE_Y * 4];
	for (int y = 0; y < GRIDSIZE_Y; ++y) {
		for (int x = 0; x < GRIDSIZE_X; ++x) {
			gridData[(GRIDSIZE_X * y + x) * 4 + 0] = 255;
			gridData[(GRIDSIZE_X * y + x) * 4 + 1] = 255;
			gridData[(GRIDSIZE_X * y + x) * 4 + 2] = 255;
			gridData[(GRIDSIZE_X * y + x) * 4 + 3] = 100;
		}
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GRIDSIZE_X, GRIDSIZE_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&gridData[0]);
}

void Grid::update() {
	glUseProgram(shaderId);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, GRIDSIZE_X, GRIDSIZE_Y, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&gridData[0]);
}

void Grid::render() {

	glUseProgram(shaderId);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glUniform1i(glGetUniformLocation(shaderId, "grid_color"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}



Grid::~Grid()
{
}
