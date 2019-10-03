#pragma once

#include <global.hpp>
#include <time.h>


class Player;

namespace mapgen {

	static const int grid_size = 128;
	static const int nIndices = 219024;
	static const int nVertices = 36895;

	static float empty_map_vertices[nVertices*10];
	float *EmptyMapVertices();

	static float map_vertices[nVertices*10];
	float *MapVertices();

	static unsigned int indices[nIndices];
	unsigned int *Indices();

	static int vertices_pos[nVertices];
	int *VerticesPos();

	void reset_map();
	float getNoiseEstimate(float x, float y);

	//--------------

	

	static std::vector<Player> *PlayerList;

	void init();
	void setPlayerList(std::vector<Player> *List);
	float Random2D(glm::vec2 st);
	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(int ind, float x, float y);
	float smoothNoise(float y, float z, float alpha = 0.005f, float beta = 1.0f);
	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed);
	void generateRandomMap();
	float generateNoise(glm::vec2 coords, bool normal = false);
	glm::vec3 updatedNormals(glm::vec2 pos);
	void define_settlements();
}