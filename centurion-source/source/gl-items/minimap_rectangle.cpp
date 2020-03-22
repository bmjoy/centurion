#include "minimap_rectangle.h"

#include <game/strategy.h>
#include <mapgen/mapgen.h>
#include <pathfinding/pathfinding.h>
#include <engine.h>

MinimapRectangle::MinimapRectangle()
{
	vPath = "assets/shaders/minimap/vertex.glsl";
	fPath = "assets/shaders/minimap/fragment.glsl";
}

void MinimapRectangle::create() {
	glUseProgram(shaderId);
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	float w = Engine::myWindow::Width;
	float h = Engine::myWindow::Height;

	float vertices[] = {
		// X,Y,Z			// uv coords		
		0.f, 0.f, 0.f,		0.f, 0.f,  // in basso a sx
		w, 0.f, 0.f,		1.f, 0.f,  // in basso a dx
		w, h, 0.f,			1.f, 1.f,  // in alto a dx
		0.f, h, 0.f,		0.f, 1.f   // in alto a sx
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

	unsigned char* imageData = new unsigned char[int(w * h * 4)];

	imageData = { 0 };
	textureIdList.push_back(0);

	glGenTextures(1, &textureIdList[0]);
	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, int(w), int(h), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
}

void MinimapRectangle::update() {
	glUseProgram(shaderId);
	
	/* This texture comes from the minimap */

	int w = (int)Engine::myWindow::Width;
	int h = (int)Engine::myWindow::Height;
	unsigned char* imageData = new unsigned char[int(w * h * 4)];
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)imageData);
}

void MinimapRectangle::render() {

	glUseProgram(shaderId);

	/* Draw */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

MinimapRectangle::~MinimapRectangle()
{
}
