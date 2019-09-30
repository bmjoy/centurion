#pragma once

#include <global.hpp>
#include <time.h>


struct noiseData {
	float zNoise;
	float grassWeight = 1.0f;
	float roadWeight = 0.0f;
};

class Player;

namespace mapgen {

	static float empty_map_vertices[368950];
	float *EmptyMapVertices();

	static float map_vertices[368950];
	float *MapVertices();

	static unsigned int indices[219024];
	unsigned int *Indices();

	static int vertices_pos[36895];
	int *VerticesPos();

	void reset_map();

	//--------------

	static std::vector<Player> *PlayerList;

	void init();
	void setPlayerList(std::vector<Player> *List);
	float Random2D(glm::vec2 st);
	float Interpolate(float a, float b, float x);
	float InterpolatedNoise(int ind, float x, float y);
	float smoothNoise(float y, float z, float alpha = 0.005f, float beta = 1.0f);
	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed);
	noiseData generateNoise(glm::vec2 coords, bool normal = false);
	glm::vec3 updatedNormals(glm::vec2 pos);
	void define_settlements();
}