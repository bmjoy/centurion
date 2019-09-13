#include "empty_rectangle.h"

EmptyRectangle::EmptyRectangle() {
	vPath = "assets/shaders/empty_rectangle/vertex.glsl";
	fPath = "assets/shaders/empty_rectangle/fragment.glsl";
}

void EmptyRectangle::init() {

	for (int i = 0; i < 24; i++) {
		empty_vertices[i] = 0.0f;
	}

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(empty_vertices), empty_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void EmptyRectangle::create(std::array<float, 8> coords) {

	/*
	x0,y0------x3,y3
	|          |
	|          |
	|          |
	x1,y1------x2,y2
	*/

	/*
	positions
	x0,		y0,		0.0f,
	x1,		y1,		0.0f,
	//------------------
	x1,		y1,		0.0f,
	x2,		y2,		0.0f,
	//------------------
	x2,		y2,		0.0f,
	x3,		y3,		0.0f,
	//------------------
	x3,		y3,		0.0f,
	x0,		y0,		0.0f,
	*/

	vertices[0] = (coords[0]); vertices[1] = (coords[1]); vertices[2] = (0.0f);
	vertices[3] = (coords[2]); vertices[4] = (coords[3]-1); vertices[5] = (0.0f);

	vertices[6] = (coords[2]); vertices[7] = (coords[3]); vertices[8] = (0.0f);
	vertices[9] = (coords[4]); vertices[10] = (coords[5]); vertices[11] = (0.0f);

	vertices[12] = (coords[4]); vertices[13] = (coords[5]); vertices[14] = (0.0f);
	vertices[15] = (coords[6]); vertices[16] = (coords[7]); vertices[17] = (0.0f);

	vertices[18] = (coords[6]); vertices[19] = (coords[7]); vertices[20] = (0.0f);
	vertices[21] = (coords[0]); vertices[22] = (coords[1]); vertices[23] = (0.0f);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void EmptyRectangle::render(glm::mat4 viewMat, glm::mat4 modelMat, glm::vec4 color) {
	glUseProgram(shaderId);

	/* Uniform Variables */

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniform4f(glGetUniformLocation(shaderId, "BorderColor"), color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, color.w);

	/* Draw */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glDrawArrays(GL_LINES, 0, 8);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

EmptyRectangle::~EmptyRectangle()
{
}