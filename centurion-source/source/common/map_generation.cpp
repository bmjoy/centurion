#include <map_generation.h>
#include "../player/player.h"

/* ----- NOISE FUNCTIONS ----- */

// the main function is Perlin().
// Perlin(x,y) takes a couple of coordinates x,y and returns a z-value 
// that is "random" and continuous with the near coordinates

// the algorithm is based on "Perlin Noise" algorithm
// check also "Simplex Noise" for a different one but similar result.

void mapgen::init() {
	*mapgen::VerticesPos() = { 0 };
	*mapgen::Indices() = { 0 };
	*mapgen::EmptyMapVertices() = { 0.f };
	*mapgen::MapVertices() = { 0.f };
}

float *mapgen::EmptyMapVertices(){
	return empty_map_vertices;
}

float *mapgen::MapVertices(){
	return map_vertices;
}

unsigned int *mapgen::Indices(){
	return indices;
}

int *mapgen::VerticesPos(){
	return vertices_pos;
}

void mapgen::reset_map() {
	for (int i = 0; i < nVertices*10; i++) {
		mapgen::MapVertices()[i] = mapgen::EmptyMapVertices()[i];
	}
}

//-------------

void mapgen::setPlayerList(std::vector<Player> *List) {
	PlayerList = List;
}

float mapgen::Random2D(glm::vec2 st) {
	float value = glm::fract(sin(glm::dot(st, glm::vec2(12.9898, 78.233)))* 43758.5453123);
	return value;
}

float mapgen::Interpolate(float a, float b, float x) {  // cosine interpolation
	float ft = x * 3.1415927f;
	float f = (1 - cos(ft)) * 0.5;
	return  a * (1 - f) + b * f;
}

float mapgen::InterpolatedNoise(int ind, float x, float y) {
	int integer_X = int(floor(x));
	float fractional_X = glm::fract(x);
	int integer_Y = int(floor(y));
	float fractional_Y = glm::fract(y);
	glm::vec2 randomInput = glm::vec2(integer_X, integer_Y);
	float v1 = Random2D(randomInput);
	float v2 = Random2D(randomInput + glm::vec2(1.0, 0.0));
	float v3 = Random2D(randomInput + glm::vec2(0.0, 1.0));
	float v4 = Random2D(randomInput + glm::vec2(1.0, 1.0));
	float i1 = Interpolate(v1, v2, fractional_X);
	float i2 = Interpolate(v3, v4, fractional_X);
	return Interpolate(i1, i2, fractional_Y);
}

float mapgen::smoothNoise(float y, float z, float alpha, float beta) {
	float zSmoothed;
	zSmoothed = z * beta * tanh(y * alpha) * tanh((GAME::MAP_HEIGHT - y)*alpha);
	return zSmoothed;
}

float mapgen::perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed) {
	z_scale *= GAME::MAP_WIDTH;
	xy_scale /= GAME::MAP_WIDTH;

	x += x_seed;
	y += y_seed;
	x *= xy_scale;
	y *= xy_scale;
	int numOctaves = 1;
	float persistence = 0.5;
	float total = 0,
		frequency = pow(2, numOctaves),
		amplitude = 1;
	for (int i = 0; i < numOctaves; ++i) {
		frequency /= 2;
		amplitude *= persistence;
		total += InterpolatedNoise(int(i % 10), x / frequency, y / frequency) * amplitude;
	}
	float z = total / frequency * z_scale;
	
	return z;
}

float mapgen::generateNoise(glm::vec2 coords, bool normal) {
	float zscale = MAP::ZSCALE;
	
	normal ? zscale *= 1.5f : zscale *= 2.f;
	float zNoise = perlinNoise(coords.x, coords.y, MAP::XYSCALE, zscale, MAP::XSEED, MAP::YSEED);
	return zNoise;
}

glm::vec3 getVertex(int x, int y) {

	int j = mapgen::VerticesPos()[(int)(y/mapgen::grid_size * GAME::MAP_WIDTH / mapgen::grid_size + x/ mapgen::grid_size)];

	float zNoise = mapgen::MapVertices()[j * 10 + 3] * 2;
	float x1 = mapgen::MapVertices()[j * 10];
	float y1 = mapgen::MapVertices()[j * 10 + 1] + zNoise;
	float z1 = mapgen::MapVertices()[j * 10 + 2] + zNoise;
	return glm::vec3(x1, y1, z1);
}

std::array<glm::vec3, 6> getAdjacentVertices(glm::vec2 pos) {

	std::array<glm::vec3, 6> output;

	output = {

		getVertex(pos.x, pos.y - mapgen::grid_size), // down
		getVertex(pos.x - mapgen::grid_size, pos.y), // left
		getVertex(pos.x - mapgen::grid_size, pos.y + mapgen::grid_size), // up-left
		getVertex(pos.x, pos.y + mapgen::grid_size), // up
		getVertex(pos.x + mapgen::grid_size, pos.y), // right	
		getVertex(pos.x + mapgen::grid_size, pos.y - mapgen::grid_size), // down-right
	};
	return output;
}

glm::vec3 mapgen::updatedNormals(glm::vec2 pos) {
	glm::vec3 off = glm::vec3(2.0, 2.0, 0.0);

	float hL = generateNoise(glm::vec2(pos.x - off.x, pos.y), true);
	float hR = generateNoise(glm::vec2(pos.x + off.x, pos.y), true);
	float hD = generateNoise(glm::vec2(pos.x, pos.y - off.y), true);
	float hU = generateNoise(glm::vec2(pos.x, pos.y + off.y), true);
	float Nx = hL - hR;
	float Ny = hD - hU;
	float Nz = 2.f;
	glm::vec3 N = glm::normalize(glm::vec3(Nx, Ny, Nz));
	return N;

	//if (pos.x > 128 && pos.x < 29952 - 128 && pos.y > 128 && pos.y < 19968 - 128) {
	//	glm::vec3 a = getVertex(pos.x, pos.y);

	//	std::array<glm::vec3, 6> adjVertices = getAdjacentVertices(pos);

	//	glm::vec3 sum(0.f);

	//	for (int i = 0; i < 6; i++) {
	//		int j;
	//		(i == 0) ? j = 5 : j = i - 1;

	//		glm::vec3 side1 = adjVertices[i];
	//		glm::vec3 side2 = adjVertices[j];

	//		glm::vec3 normal = glm::cross(side1, side2);
	//		//normal = glm::normalize(normal);

	//		sum += normal;

	//	}
	//	glm::vec3 output = glm::normalize(sum);

	//	return output;
	//}

	//else {
	//	return glm::vec3(0,0,1);
	//}
}

void mapgen::generateRandomMap() {
	for (int i = 0; i < nVertices*10; i+=10) {
		float xCoord = mapgen::MapVertices()[i];
		float yCoord = mapgen::MapVertices()[i + 1];

		float zNoise = generateNoise(glm::vec2(xCoord, yCoord), false);
		zNoise = mapgen::smoothNoise(yCoord, zNoise);

		// update znoise
		mapgen::MapVertices()[i + 3] = zNoise;
		MAP::MIN_Z = std::min(zNoise, MAP::MIN_Z);
		MAP::MAX_Z = std::max(zNoise, MAP::MAX_Z);

		// normals
		glm::vec3 N = mapgen::updatedNormals(glm::vec2(xCoord, yCoord));
		mapgen::MapVertices()[i + 6] = N.x;
		mapgen::MapVertices()[i + 7] = N.y;
		mapgen::MapVertices()[i + 8] = N.z;
	}
}

float mapgen::getNoiseEstimate(float x, float y) {

	// bottom left
	int x0 = (int)x / mapgen::grid_size;
	int y0 = (int)y / mapgen::grid_size;
	int j0 = mapgen::VerticesPos()[(int)(y0 * GAME::MAP_WIDTH / mapgen::grid_size + x0)] * 10 + 3;
	float z0 = mapgen::MapVertices()[j0];

	// bottom right
	int x1 = x0 + 1;
	int y1 = y0;
	int j1 = mapgen::VerticesPos()[(int)(y1 * GAME::MAP_WIDTH / mapgen::grid_size + x1)] * 10 + 3;
	float z1 = mapgen::MapVertices()[j1];

	// top right
	int x2 = x1;
	int y2 = y1 + 1;
	int j2 = mapgen::VerticesPos()[(int)(y2 * GAME::MAP_WIDTH / mapgen::grid_size + x2)] * 10 + 3;
	float z2 = mapgen::MapVertices()[j2];

	// top left
	int x3 = x0;
	int y3 = y2;
	int j3 = mapgen::VerticesPos()[(int)(y3 * GAME::MAP_WIDTH / mapgen::grid_size + x3)] * 10 + 3;
	float z3 = mapgen::MapVertices()[j3];

	// deltas
	float dx = x / mapgen::grid_size - x0;
	float dy = y / mapgen::grid_size - y0;

	// values 
	float v0 = z0 * (1 - dx) + z1 * dx; // bottom
	float v1 = z1 * (1 - dy) + z2 * dy; // right
	float v2 = z3 * (1 - dx) + z2 * dx; // top
	float v3 = z0 * (1 - dy) + z3 * dy; // left

	// average
	float zHat = (v0 * (1 - dy) + v1 * dx + v2 * dy + v3 * (1 - dx)) / 2.f;

	return zHat;
}

void mapgen::define_settlements() {

	srand(time(NULL));
	std::vector<float> townhallPos;
	int a, b;
	bool c;
	float d;
	int k = 0;
	int min = GAME::TOWNHALL_RADIUS + 100;
	int max_X = GAME::MAP_WIDTH - GAME::TOWNHALL_RADIUS - 100;
	int max_Y = GAME::MAP_HEIGHT - GAME::TOWNHALL_RADIUS - 100;
	for (int n = 0; n < (*PlayerList).size(); n++) {
		c = false;
		while (!c) {
			a = (float)(rand() % (max_X - min) + min);
			b = (float)(rand() % (max_Y - min) + min);

			if (n == 0) { // 1 PLAYER
				if (Distance(a, b, GAME::MAP_WIDTH/2.0f, GAME::MAP_HEIGHT/2.0f) > GAME::MAP_HEIGHT / 2.0f) {
					townhallPos.push_back(a);
					townhallPos.push_back(b);
					c = true;
				}
			}
			if (n == 1) { // 2 PLAYERS
				d = Distance(a, b, townhallPos[n - 1], townhallPos[n]);
				if (d > GAME::MAP_HEIGHT * 0.5) {
					townhallPos.push_back(a);
					townhallPos.push_back(b);
					c = true;
				}
			}
			if (n == 2 || n == 3) { // 3 & 4 PLAYERS
				bool c2 = true;
				for (int m = n - 1; m >= 0; m--) {
					d = Distance(a, b, townhallPos[m * 2], townhallPos[m * 2 + 1]);
					if (d <= GAME::MAP_HEIGHT * 0.5) {
						c2 *= false;
					}
				}
				if (c2) {
					townhallPos.push_back(a);
					townhallPos.push_back(b);
					c = true;
				}
			}
			if (n > 3) { // MORE THAN 4 PLAYERS

				bool c2 = true;
				for (int m = n - 1; m >= 0; m--) {
					d = Distance(a, b, townhallPos[m * 2], townhallPos[m * 2 + 1]);
					if (d <= GAME::MAP_HEIGHT * 0.25) {
						c2 *= false;
					}
				}
				if (c2) {
					townhallPos.push_back(a);
					townhallPos.push_back(b);
					c = true;
				}
			}
		}
	}

	for (int i = 0; i < (*PlayerList).size(); i++) {
		(*PlayerList)[i].setStartPoint(glm::vec2(townhallPos[i * 2], townhallPos[i * 2 + 1]));
	}
	//return townhallPos;
}
