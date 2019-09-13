#pragma once

#include <global.hpp>
#include <shader.h>

class EmptyRectangle : public Shader
{
public:
	EmptyRectangle();
	void init();
	void create(std::array<float, 8> coords);
	void render(glm::mat4 viewMat, glm::mat4 modelMat, glm::vec4 color = glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));
	~EmptyRectangle();
private:
	float empty_vertices[24];
	float vertices[24];
};