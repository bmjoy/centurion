#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader.h"
#include "light.h"

struct terrainTexture {
	vector<string> zones;
	vector<float> frequencies;
	string name;
	int id;
};

class Terrain : public Shader
{
public:
	Terrain();
	void addPath(string terrainID, string path) { terrainPathMap[terrainID] = path; }
	vector<string> getTerrainList() { return texturesName; }
	void create();
	void updateHeightsBuffer();
	void updateTextureBuffer();
	void render(bool tracing);
	~Terrain();

private:
	map<string, string> terrainPathMap;
	float randomX, randomY;
	GLuint VerticesVBO, HeightsVBO, TexturesVBO;
	int randomPos;
	void genBuffers();
	void ReadIndicesData(void);
	void ReadVerticesData(void);
	void ReadVerticesPosData(void);
	vector<string> texturesName;
	float width, height;

	// light 
	Light light = Light(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

static Terrain terrain;

#endif