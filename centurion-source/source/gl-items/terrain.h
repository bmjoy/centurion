/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

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

/// <summary>
/// This class is used to create and render the terrain, that's a 3d plane 
/// </summary>
class Terrain : public Shader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	Terrain();

	/// <summary>
	/// This function adds a terrain texture path to the path map
	/// </summary>
	/// <param name="terrainID">The terrain ID</param>
	/// <param name="path">The terrain texture path</param>
	void addPath(string terrainID, string path) { terrainPathMap[terrainID] = path; }

	/// <summary>
	/// This function returns the list of textures names
	/// </summary>
	vector<string> getTerrainList() { return texturesName; }

	/// <summary>
	/// Create function.
	/// </summary>
	void create();

	/// <summary>
	/// This function updates the Heights buffer (GPU)
	/// </summary>
	void updateHeightsBuffer();

	/// <summary>
	/// This function updates the textures buffer (GPU)
	/// </summary>
	void updateTextureBuffer();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="tracing">Boolean; true = tracing is active</param>
	void render(bool tracing);

	/// <summary>
	/// Destructor
	/// </summary>
	~Terrain();
private:
	map<string, string> terrainPathMap;
	float randomX, randomY;
	GLuint VerticesVBO, HeightsVBO, TexturesVBO;
	int randomPos;
	void genBuffers();

	/// <summary>
	/// TODO
	/// </summary>
	void ReadIndicesData(void);

	/// <summary>
	/// TODO
	/// </summary>
	void ReadVerticesData(void);

	/// <summary>
	/// TODO
	/// </summary>
	void ReadVerticesPosData(void);
	vector<string> texturesName;
	float width, height;

	// light 
	Light light = Light(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

static Terrain terrain;

#endif