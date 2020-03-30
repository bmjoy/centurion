#pragma once

#include <header.h>

/*------------------------------------------------------
	These variables contain the information about the map:

	vertices[] = xCoord,  yCoord, xTexCoord, yTexCoord
	heights[] = zNoise, xNorm, yNorm, zNorm
	textures[] = textureType
------------------------------------------------------*/

struct terrainTexture;

namespace mapgen {
	extern float mouseZNoise; // for 2d coord calculation
	extern float minZ;
	extern float maxZ;

	static const int grid_size = 128;
	static const int nIndices = 228480;
	static const int nVertices = 38479;
	static const int mapMeshWidth = 30464;  // 29952 + 128*4
	static const int mapMeshHeight = 20480; // 19968 + 128*4
	static float map_vertices[nVertices * 4];
	static float map_textures[nVertices];
	static float map_heights[nVertices * 4];
	static unsigned int indices[nIndices];
	static int vertices_pos[nVertices];

	extern std::map<std::string, std::vector<std::string>> zonesMap;
	extern std::map<std::string, terrainTexture> terrainsMap;

	//
	//	MAP GENERATION FUNCTIONS
	//


	struct Triangle {
		glm::vec3 a, b, c;
	};
	float *MapVertices();
	float *MapTextures();
	float *MapHeights();
	unsigned int *Indices();
	int *VerticesPos();
	void reset_map();
	float getNoiseEstimate(float x, float y);
	int getVertexPos(int x, int y);
	glm::vec2 getVertexCoords(int j);
	glm::vec3 getVertex(int x, int y, bool isNormal);
	std::array<Triangle, 6> getAdjacentTriangles(glm::ivec2 pos);
	glm::vec3 calculateTriangleNormal(Triangle T);
	std::vector<glm::ivec2> getAdjacentVertices(glm::ivec2 pos);
	void updateAllNormals();
	void updateNormal(int x, int y);

	void init();
	float Random2D(glm::vec2 st);
	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(int ind, float x, float y);
	float smoothNoise(float y, float z, float alpha = 0.005f, float beta = 1.0f);
	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed);
	void generateRandomMap();
	float generateNoise(glm::vec2 coords, std::string type);
	void define_buildings_location(int num_players, int num_outposts, std::vector<glm::vec2> *outpostslocation, std::vector<glm::vec2> *townhallslocation);
	void defineTerrainZones();
};
