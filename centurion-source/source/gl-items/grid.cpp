#include "grid.h"

#include <game/strategy.h>
#include <mapgen/mapgen.h>
#include <pathfinding/pathfinding.h>



Grid::Grid()
{
	vPath = "assets/shaders/terrain/vertex_grid.glsl";
	fPath = "assets/shaders/terrain/fragment_grid.glsl";
	textureID = 0;
	gridSizeX = 0;
	gridSizeY = 0;
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

	gridSizeX = MEDIUM_MAP_WIDTH / astar::cellGridSize;
	gridSizeY = MEDIUM_MAP_HEIGHT / astar::cellGridSize;
	unsigned char* gridData = new unsigned char[gridSizeX * gridSizeY * 4];

	gridData = { 0 };

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gridSizeX, gridSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)gridData);
}

void Grid::reset() {
	glUseProgram(shaderId);
	unsigned char* gridData = new unsigned char[gridSizeX * gridSizeY * 4];
	for (int y = 0; y < gridSizeY; ++y) {
		for (int x = 0; x < gridSizeX; ++x) {
			gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 0] = 255;
			gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 1] = 255;
			gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 2] = 255;
			gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 3] = 0;
		}
	}
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gridSizeX, gridSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)gridData);
}

void Grid::update() {
	glUseProgram(shaderId);

	/* This texture comes from the real grid of 0/1 */
	float zNoise;
	int yNoise;
	unsigned char* gridData = new unsigned char[gridSizeX * gridSizeY * 4];

	for (int y = 0; y < gridSizeY; ++y) {
		for (int x = 0; x < gridSizeX; ++x) {

			/* From 3d grid to 2d */
			/* for pathfinding calculation */
			zNoise = mapgen::generateNoise(glm::vec2(x*astar::cellGridSize, y*astar::cellGridSize), "height");
			zNoise = mapgen::smoothNoise((float)y*astar::cellGridSize, zNoise);
			yNoise = int(y*astar::cellGridSize + zNoise) / astar::cellGridSize;
			if (yNoise < gridSizeY && yNoise > 0) {
				astar::GridMatrix2D()[y * astar::gridWidth + x] = astar::GridMatrix()[yNoise * astar::gridWidth + x];
			}

			/* save a grid image for debug and graphic purposes*/

			if (astar::GridMatrix2D()[y * astar::gridWidth + x] == 1) {
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 0] = 0;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 1] = 0;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 2] = 0;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 3] = 100;
			}
			else {
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 0] = 255;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 1] = 255;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 2] = 255;
				gridData[(gridSizeX * (gridSizeY - 1 - y) + x) * 4 + 3] = 0;
			}
		}
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gridSizeX, gridSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)gridData);
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
