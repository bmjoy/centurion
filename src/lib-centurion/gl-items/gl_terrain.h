/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GL_TERRAIN_H
#define GL_TERRAIN_H

#include "gl_shader.h"
#include "gl_light.h"

struct terrainTexture {
	std::vector<std::string> zones;
	std::vector<float> frequencies;
	std::string name;
	int id;
};

/// <summary>
/// This class is used to create and render the terrain, that's a 3d plane 
/// </summary>
class glTerrain : public glShader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	glTerrain();

	/// <summary>
	/// This function returns the list of textures names
	/// </summary>
	std::vector<std::string> getTerrainList() { return texturesName; }

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
	~glTerrain();
private:
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
	std::vector<std::string> texturesName;
	float width, height;

	// light 
	glLight light = glLight(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

#endif
