#pragma once

#include <header.h>

using namespace std;
using namespace glm;

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

	extern map<string, vector<string>> zonesMap;
	extern map<string, terrainTexture> terrainsMap;

	//
	//	MAP GENERATION FUNCTIONS
	//


	struct Triangle {
		vec3 a, b, c;
	};
	float *MapVertices();
	float *MapTextures();
	float *MapHeights();
	unsigned int *Indices();
	int *VerticesPos();
	void reset_map();
	float getNoiseEstimate(float x, float y);
	int getVertexPos(int x, int y);
	vec2 getVertexCoords(int j);
	vec3 getVertex(int x, int y, bool isNormal);
	array<Triangle, 6> getAdjacentTriangles(ivec2 pos);
	vec3 calculateTriangleNormal(Triangle T);
	vector<ivec2> getAdjacentVertices(ivec2 pos);
	void updateAllNormals();
	void updateNormal(int x, int y);

	void init();
	float Random2D(vec2 st);
	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(int ind, float x, float y);
	float smoothNoise(float y, float z, float alpha = 0.005f, float beta = 1.0f);
	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed);
	void generateRandomMap();
	float generateNoise(vec2 coords, string type);
	void define_buildings_location(int num_players, int num_outposts, vector<vec2> *outpostslocation, vector<vec2> *townhallslocation);
	void defineTerrainZones();
};