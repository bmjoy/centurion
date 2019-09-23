#include <map_generation.h>

/* ----- NOISE FUNCTIONS ----- */

// the main function is Perlin().
// Perlin(x,y) takes a couple of coordinates x,y and returns a z-value 
// that is "random" and continuous with the near coordinates

// the algorithm is based on "Perlin Noise" algorithm
// check also "Simplex Noise" for a different one but similar result.

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

noiseData mapgen::generateNoise(glm::vec2 coords, bool normal) {
	noiseData output;
	float zscale = MAP::ZSCALE;
	
	normal ? zscale *= 1.5f : zscale *= 1.f;
	output.zNoise = perlinNoise(coords.x, coords.y, MAP::XYSCALE, zscale, MAP::XSEED, MAP::YSEED);

	/* Townhalls */

	for (int i = 0; i < (*PlayerList).size(); i++) {
		float xTown = (*PlayerList)[i].getStartPoint().x;
		float yTown = (*PlayerList)[i].getStartPoint().y;
		float dE = distEllipse(coords.x, coords.y, xTown, yTown, GAME::TOWNHALL_RADIUS);
		if (dE <= 1.0) {
			output.roadWeight = 1.0f;
			output.grassWeight = 0.0f;
			output.zNoise *= (dE);
		}
	}
	return output;
}

glm::vec3 mapgen::updatedNormals(glm::vec2 pos) {
	glm::vec3 off = glm::vec3(2.0, 2.0, 0.0);

	float hL = generateNoise(glm::vec2(pos.x - off.x, pos.y), true).zNoise;
	float hR = generateNoise(glm::vec2(pos.x + off.x, pos.y), true).zNoise;
	float hD = generateNoise(glm::vec2(pos.x, pos.y - off.y), true).zNoise;
	float hU = generateNoise(glm::vec2(pos.x, pos.y + off.y), true).zNoise;
	float Nx = hL - hR;
	float Ny = hD - hU;
	float Nz = 2.f;
	glm::vec3 N = glm::normalize(glm::vec3(Nx, Ny, Nz));
	return N;
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