#pragma once

#include <global.hpp>
#include <shader.h>

class Image: public Shader
{
public:
	Image();
	Image(int shaderID);
	void apply_projection_matrix(glm::mat4 mat);
	void create(std::string image_path, std::string origin, glm::ivec2 image_size = glm::ivec2(0,0));
	void render(float xPos, float yPos, bool picking = false, int picking_id = 0);
	~Image();

private:
	float w, h;
	const char* path;       // texture path
	float x0, y0, x1, y1, x2, y2, x3, y3;
	glm::mat4 modelMat;
	int r, g, b;
};

