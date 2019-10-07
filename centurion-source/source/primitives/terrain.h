#pragma once
#include <mapgen>
#include <shader>
#include "light.h"

class Terrain : public Shader
{
public:
	Terrain();
	void create();
	void updateHeightsBuffer();
	void updateTextureBuffer();
	void render(bool tracing);
	~Terrain();

private:
	const char* path_grass;
	const char* path_road;
	const char* path_rock;
	float randomX, randomY;
	GLuint VerticesVBO, HeightsVBO, TexturesVBO;
	int randomPos;

	void genBuffers();
	void genTexture();	
	

	// light 
	Light light = Light(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

