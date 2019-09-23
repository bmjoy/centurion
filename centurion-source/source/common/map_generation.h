#pragma once

#include <global.hpp>
#include <time.h>
#include "../player/player.h"

struct noiseData {
	float zNoise;
	float grassWeight = 1.0f;
	float roadWeight = 0.0f;
};

namespace mapgen {

	static std::vector<Player> *PlayerList;

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