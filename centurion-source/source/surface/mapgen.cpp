#include <surface>
#include <math>
#include <global>
#include <game/strategy.h>
#include <player>
#include <random>
#include <ctime>



namespace mapgen {

	map<string, vector<string>> zonesMap;
	map<string, terrainTexture> terrainsMap;

	float mouseZNoise = 0.f;
	float minZ = 0.f;
	float maxZ = 0.f;

	void init() {
		*VerticesPos() = { 0 };
		*Indices() = { 0 };
		*MapVertices() = { 0.f };
		*MapHeights() = { 0.f };
		*MapTextures() = { 1.f };
	}
	float *MapVertices() {
		return map_vertices;
	}
	float *MapTextures() {
		return map_textures;
	}
	float *MapHeights() {
		return map_heights;
	}
	unsigned int *Indices() {
		return indices;
	}
	int *VerticesPos() {
		return vertices_pos;
	}

	void reset_map() {
		for (int i = 0; i < nVertices; i++) {

			/* reset textures */
			MapTextures()[i] = 1.f;

			/* reset noise and normals */
			MapHeights()[i * 4] = 0.f;
			MapHeights()[i * 4 + 1] = 0.f;
			MapHeights()[i * 4 + 2] = 0.f;
			MapHeights()[i * 4 + 3] = 0.707107f;
		}
	}

	//-------------

	float Random2D(glm::vec2 st) {
		float value = glm::fract(sin(glm::dot(st, glm::vec2(12.9898f, 78.233f)))* 43758.5453123f);
		return value;
	}

	float Interpolate(float a, float b, float x) {  // cosine interpolation
		float ft = x * 3.1415927f;
		float f = (1.0f - cos(ft)) * 0.5f;
		return  a * (1 - f) + b * f;
	}

	float InterpolatedNoise(int ind, float x, float y) {
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

	float smoothNoise(float y, float z, float alpha, float beta) {
		float zSmoothed;
		zSmoothed = z * beta * tanh(y * alpha) * tanh((mapMeshHeight - y)*alpha);
		return zSmoothed;
	}

	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed) {
		z_scale *= mapMeshWidth;
		xy_scale /= mapMeshWidth;

		x += x_seed;
		y += y_seed;
		x *= xy_scale;
		y *= xy_scale;
		int numOctaves = 1;
		float persistence = 0.5;
		float total = 0,
			frequency = pow(2.0f, numOctaves),
			amplitude = 1;
		for (int i = 0; i < numOctaves; ++i) {
			frequency /= 2;
			amplitude *= persistence;
			total += InterpolatedNoise(int(i % 10), x / frequency, y / frequency) * amplitude;
		}
		float z = total / frequency * z_scale;

		return z;
	}

	float generateNoise(vec2 coords, string type) {

		float XYSCALE, ZSCALE, XSEED, YSEED;

		if (type == "height") {
			XYSCALE = 80.0f;
			ZSCALE = 0.025f / 3.0f;
			XSEED = 0.0f;
			YSEED = 0.0f;
		}

		if (type == "texture") {
			XYSCALE = 10.0f;
			ZSCALE = 0.1f;
			XSEED = 0.0f;
			YSEED = 0.0f;
		}

		float zNoise = perlinNoise(coords.x, coords.y, XYSCALE, ZSCALE, XSEED, YSEED);
		return zNoise;
	}

	int getVertexPos(int x, int y) {
		int j = VerticesPos()[(y / grid_size * mapMeshWidth / grid_size + x / grid_size)];
		return j;
	}

	vec2 getVertexCoords(int j) {
		float x = MapVertices()[j * 4];
		float y = MapVertices()[j * 4 + 1];
		return vec2(x, y);
	}

	glm::vec3 getVertex(int x, int y, bool isNormal) {
		int j = getVertexPos(x, y);
		float intensity;
		isNormal ? intensity = 1.5f : intensity = 1.f;
		float zNoise = MapHeights()[j * 4] * intensity;
		float x1 = (float)x;
		float y1 = (float)y + zNoise;
		float z1 = (float)y*(-1.f) + zNoise;
		return glm::vec3(x1, y1, z1);
	}

	std::array<Triangle, 6> getAdjacentTriangles(glm::ivec2 pos) {
		int gap = grid_size;
		std::array<Triangle, 6> triangles;
		triangles[0] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x - gap, pos.y, true),getVertex(pos.x - gap, pos.y + gap, true),
		};
		triangles[1] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x - gap, pos.y + gap, true), getVertex(pos.x, pos.y + gap, true)
		};
		triangles[2] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x, pos.y + gap, true), getVertex(pos.x + gap, pos.y, true)
		};
		triangles[3] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x + gap, pos.y, true), getVertex(pos.x + gap, pos.y - gap, true)
		};
		triangles[4] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x + gap, pos.y - gap, true),getVertex(pos.x, pos.y - gap, true),
		};
		triangles[5] = {
			getVertex(pos.x, pos.y, true), getVertex(pos.x, pos.y - gap, true), getVertex(pos.x - gap, pos.y, true)
		};
		return triangles;
	}

	glm::vec3 calculateTriangleNormal(Triangle T) {
		glm::vec3 N = glm::cross(T.c - T.a, T.b - T.a);
		return N;
	}

	std::vector<glm::ivec2> getAdjacentVertices(glm::ivec2 pos) {
		int gap = grid_size;
		std::vector<glm::ivec2> output;
		output = {
			glm::ivec2(pos.x - gap, pos.y), // left
			glm::ivec2(pos.x - gap, pos.y + gap), // up-left
			glm::ivec2(pos.x, pos.y + gap), // up
			glm::ivec2(pos.x + gap, pos.y), // right 
			glm::ivec2(pos.x + gap, pos.y - gap), // down-right
			glm::ivec2(pos.x, pos.y - gap), // down
		};
		return output;
	}

	void updateNormal(int x, int y) {
		std::vector<glm::ivec2> vertices = getAdjacentVertices(glm::ivec2(x, y));
		vertices.push_back(glm::ivec2(x, y));

		for (int i = 0; i < 7; i++) {
			glm::ivec2 pos = glm::ivec2(vertices[i].x, vertices[i].y);
			int j = getVertexPos(pos.x, pos.y);

			if (pos.x > grid_size && pos.x < mapMeshWidth - grid_size && pos.y > grid_size && pos.y < mapMeshHeight - grid_size) {

				std::array<Triangle, 6> adjacentTriangles = getAdjacentTriangles(pos);
				glm::vec3 sum(0.f);
				for (int k = 0; k < 6; k++) {
					glm::vec3 normal = calculateTriangleNormal(adjacentTriangles[k]);
					sum += normal;
				}
				glm::vec3 N = glm::normalize(sum);

				MapHeights()[j * 4 + 1] = N.x;
				MapHeights()[j * 4 + 2] = N.y;
				MapHeights()[j * 4 + 3] = N.z;
			}
			else {
				MapHeights()[j * 4 + 1] = 0.f;
				MapHeights()[j * 4 + 2] = 0.f;
				MapHeights()[j * 4 + 3] = 0.707107f;
			}
		}
	}

	void updateAllNormals() {
		for (int i = 0; i < nVertices; i++) {
			float xCoord = MapVertices()[i * 4];
			float yCoord = MapVertices()[i * 4 + 1];
			glm::ivec2 pos = glm::ivec2(int(xCoord), int(yCoord));
			if (pos.x > grid_size && pos.x < mapMeshWidth - grid_size && pos.y > grid_size && pos.y < mapMeshHeight - grid_size) {
				std::array<Triangle, 6> adjacentTriangles = getAdjacentTriangles(pos);
				glm::vec3 sum(0.f);

				for (int k = 0; k < 6; k++) {
					glm::vec3 normal = calculateTriangleNormal(adjacentTriangles[k]);
					sum += normal;
				}
				glm::vec3 N = glm::normalize(sum);

				MapHeights()[i * 4 + 1] = N.x;
				MapHeights()[i * 4 + 2] = N.y;
				MapHeights()[i * 4 + 3] = N.z;
			}
			else {
				MapHeights()[i * 4 + 1] = 0.f;
				MapHeights()[i * 4 + 2] = 0.f;
				MapHeights()[i * 4 + 3] = 0.707107f;
			}
		}
	}

	void generateRandomMap() {
		//random_device rd;
		//mt19937 gen(rd());
		//uniform_real_distribution<float> distribution(0, 3);

		for (int i = 0; i < nVertices; i++) {
			float xCoord = MapVertices()[i * 4];
			float yCoord = MapVertices()[i * 4 + 1];

			float zNoise = generateNoise(glm::vec2(xCoord, yCoord), "height");
			zNoise = smoothNoise(yCoord, zNoise);

			//float zTex = generateNoise(glm::vec2(xCoord, yCoord), "texture");
			//zTex = smoothNoise(yCoord, zTex);

			// update znoise
			MapHeights()[i * 4] = zNoise;
			minZ = std::min(zNoise, minZ);
			maxZ = std::max(zNoise, maxZ);

			// terrain
			//float p = distribution(gen);
			//if (p <= 1.5) MapTextures()[i] = 1.f;
			//if (p > 1.5 && p <= 2.5) MapTextures()[i] = 2.f;
			//if (p > 2.5 && p <= 3) MapTextures()[i] = 4.f;
			

			// update texture
			//if (zTex > 1000)
				//MapTextures()[i] = 10.f;
		}
		updateAllNormals();
	}

	float getNoiseEstimate(float x, float y) {

		int gap = grid_size;

		// bottom left
		int x0 = (int)x;
		int y0 = (int)y;
		int j0 = getVertexPos(x0, y0) * 4;
		float z0 = MapHeights()[j0];

		// bottom right
		int x1 = x0 + gap;
		int y1 = y0;
		int j1 = getVertexPos(x1, y1) * 4;
		float z1 = MapHeights()[j1];

		// top right
		int x2 = x1;
		int y2 = y1 + gap;
		int j2 = getVertexPos(x2, y2) * 4;
		float z2 = MapHeights()[j2];

		// top left
		int x3 = x0;
		int y3 = y2;
		int j3 = getVertexPos(x3, y3) * 4;
		float z3 = MapHeights()[j3];

		// deltas
		float dx = x / grid_size - x0 / grid_size;
		float dy = y / grid_size - y0 / grid_size;

		// values 
		float v0 = z0 * (1 - dx) + z1 * dx; // bottom
		float v1 = z1 * (1 - dy) + z2 * dy; // right
		float v2 = z3 * (1 - dx) + z2 * dx; // top
		float v3 = z0 * (1 - dy) + z3 * dy; // left

		// average
		float zHat = (v0 * (1 - dy) + v1 * dx + v2 * dy + v3 * (1 - dx)) / 2.f;

		return zHat;
	}

	void define_buildings_location(int num_players, int num_outposts, vector<vec2> *outpostslocation, vector<vec2> *townhallslocation) {
		default_random_engine generator;
		srand((unsigned int)time(NULL));
		vector<vec2> townhallPos(num_players, vec2(0));
		vector<vec2> outpostsPos(num_outposts, vec2(0));

		/* locations grid definition */
		vector<vec2> locGrid;
		int min = TOWNHALL_RADIUS + grid_size;
		int max_X = mapMeshWidth - TOWNHALL_RADIUS - grid_size;
		int max_Y = mapMeshHeight - TOWNHALL_RADIUS - grid_size;
		for (int iHoriz = 0; iHoriz < mapMeshWidth - grid_size * 4; iHoriz += grid_size) {
			for (int iVert = 0; iVert < mapMeshHeight - grid_size * 4; iVert += grid_size) {
				locGrid.push_back(vec2(iHoriz, iVert));
			}
		}

		/* townhalls */

		for (int n = 0; n < num_players; n++) {
			bool c = false;
			while (!c) {
				uniform_int_distribution<int> distribution(0, int(locGrid.size()) - 1);
				int j = distribution(generator);

				float a = locGrid[j].x;
				float b = locGrid[j].y;

				if ((a >= mapMeshWidth - TOWNHALL_RADIUS * 1.1) || (a <= TOWNHALL_RADIUS * 1.1) ||
					(b >= mapMeshHeight - TOWNHALL_RADIUS * 1.1) || (b <= TOWNHALL_RADIUS * 1.1)) continue;

				if (n == 0) { // 1 PLAYER
					if (math::euclidean_distance(a, b, mapMeshWidth / 2.0f, mapMeshHeight / 2.0f) > mapMeshHeight / 2.0f) {
						townhallPos[n].x = a;
						townhallPos[n].y = b;
						c = true;
					}
				}
				if (n == 1) { // 2 PLAYERS
					float d = math::euclidean_distance(a, b, townhallPos[0].x, townhallPos[0].y);
					if (d > mapMeshHeight * 0.5) {
						townhallPos[n].x = a;
						townhallPos[n].y = b;
						c = true;
					}
				}
				if (n == 2 || n == 3) { // 3 & 4 PLAYERS
					bool c2 = true;
					for (int m = n - 1; m >= 0; m--) {
						float d = math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y);
						if (d <= mapMeshHeight * 0.5) {
							c2 = c2 * false;
						}
					}
					if (c2) {
						townhallPos[n].x = a;
						townhallPos[n].y = b;
						c = true;
					}
				}
				if (n > 3) { // MORE THAN 4 PLAYERS

					bool c2 = true;
					for (int m = n - 1; m >= 0; m--) {
						float d = math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y);
						if (d <= mapMeshHeight * 0.25) {
							c2 = c2 * false;
						}
					}
					if (c2) {
						townhallPos[n].x = a;
						townhallPos[n].y = b;
						c = true;
					}
				}
			}
		}
		(*townhallslocation) = townhallPos;

		// update locGrid;

		for (int i = 0; i < locGrid.size(); i++) {
			for (int j = 0; j < townhallPos.size(); j++) {
				float d = math::euclidean_distance(locGrid[i].x, locGrid[i].y, townhallPos[j].x, townhallPos[j].y);
				if (d <= TOWNHALL_RADIUS * 1.25f)
					locGrid.erase(locGrid.begin() + i);
			}
		}

		/* outposts */

		for (int n = 0; n < num_outposts; n++) {
			bool c = false;
			while (!c) {
				uniform_int_distribution<int> distribution(0, int(locGrid.size()) - 1);
				int j = distribution(generator);

				float a = locGrid[j].x;
				float b = locGrid[j].y;

				if ((a >= mapMeshWidth - OUTPOST_RADIUS * 1.25f) || (a <= OUTPOST_RADIUS * 1.25f) ||
					(b >= mapMeshHeight - OUTPOST_RADIUS * 1.25f) || (b <= OUTPOST_RADIUS * 1.25f)) continue;

				bool c2 = true;
				for (int m = 0; m < townhallPos.size(); m++) {
					if (math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y) < TOWNHALL_RADIUS + 2 * OUTPOST_RADIUS){
						c2 = c2 * false;
					}
				}
				for (int m = n - 1; m >= 0; m--) {
					if (math::euclidean_distance(a, b, outpostsPos[m].x, outpostsPos[m].y) <= OUTPOST_RADIUS * 4){
						c2 = c2 * false;
					}
				}
				if (c2) {
					outpostsPos[n].x = a;
					outpostsPos[n].y = b;
					c = true;
				}
			}
		}
		(*outpostslocation) = outpostsPos;
	}

	void defineTerrainZones() {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> distribution(-500.f, 500.f);
		

		for (int i = 0; i < nVertices; i++) {

			string zoneType = "none";

			float d = mapMeshWidth * 2;
			vec2 p = getVertexCoords(i) - 2.f * grid_size;

			for (map<int, Building*>::iterator settl = independent_buildings.begin(); settl != independent_buildings.end(); settl++) {
				Building* b = settl->second;
				if (b->get_class().substr(1) != "townhall") continue;

				float rnoise = distribution(gen);

				float dist = math::euclidean_distance(p.x, p.y, b->get_position().x, b->get_position().y);
				dist += rnoise;
				if (dist <= d) {
					d = dist;
					string race = b->get_race();
					if (dist - rnoise > TOWNHALL_RADIUS - grid_size) zoneType = glb::RACES[race].getEnvironmentalZone();
					else zoneType = "none";
				}
			}
			
			if (zoneType != "none"){
				vector<string> availableTerrainTypes = zonesMap[zoneType];
				vector<float> weightedTerrainsId;
				for (int t = 0; t < availableTerrainTypes.size(); t++) {
					terrainTexture terr = terrainsMap[availableTerrainTypes[t]];
					for (int z = 0; z < terr.zones.size(); z++) {
						if (terr.zones[z] == zoneType) {
							for (int f = 0; f < terr.frequencies[z]; f++)
								weightedTerrainsId.push_back(float(terr.id));
						}
					}
				}
				uniform_int_distribution<int> distribution(0, int(weightedTerrainsId.size()-1));
				int j = distribution(gen);
				MapTextures()[i] = weightedTerrainsId[j];
			}
		}
		// update texture buffer
		obj::MapTerrain()->updateTextureBuffer();
	}
};
