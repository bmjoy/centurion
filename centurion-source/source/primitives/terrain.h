#pragma once
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
#include <map_generation.h>
#include <shader.h>
#include "light.h"

class Terrain : public Shader
{
public:
	Terrain();
	void create();
	void createNoise();
	void updateBuffers(float x, float y, std::string type, float q);
	void render(bool tracing);
	~Terrain();

private:
	const char* path_grass;
	const char* path_road;
	const char* path_rock;
	const char* plane_path;

	float randomX, randomY;
	int randomPos;

	void genBuffers();
	
	void texture();

	int nVertices, nIndices;

	std::map<std::string, int> verticesPosMap;

	// light 
	Light light = Light(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

