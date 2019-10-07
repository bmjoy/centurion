#include <global.hpp>
#include <mapgen>
#include <math>
#include <objects>

/* ----- GLOBAL ----- */

namespace glb {
	void setErrors(map<string, string> errorsMap) { errors = errorsMap; }
	string getErrorCode(string error) { return errors[error]; }
	void setParam(string param, float value) { params[param] = value; }
	float getParam(string param) { return params[param]; }
	void setBoolean(string param, bool value) { booleans[param] = value; }
	bool getBoolean(string param) { return booleans[param]; }
}

/* ----- MATH ----- */

namespace math {
    float euclidean_distance(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	float ellipse_distance(float x, float y, float cx, float cy, float r) {
		return ((x - cx)*(x - cx) / (r*r) + 1.5f * (y - cy) * (y - cy) / (r * r));
	}
	/* function for empty rectangle creation*/
	std::array<float, 8> get_rectangle_coords(float x0, float y0, float w, float h) {
		return { x0, y0, x0, y0 - h, x0 + w, y0 - h, x0 + w, y0 };
	}
}

/* ----- MAP GENERATION ----- */


namespace mapgen {
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
			MapHeights()[i * 4 + 3] = 1.f;
		}
	}

	//-------------

	void setPlayerList(std::vector<Player> *List) {
		PlayerList = List;
	}

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
		zSmoothed = z * beta * tanh(y * alpha) * tanh((GAME::MAP_HEIGHT - y)*alpha);
		return zSmoothed;
	}

	float perlinNoise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed) {
		z_scale *= GAME::MAP_WIDTH;
		xy_scale /= GAME::MAP_WIDTH;

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

	float generateNoise(glm::vec2 coords, bool normal) {
		float zscale = MAP::ZSCALE;

		normal ? zscale *= 1.5f : zscale *= 1.f;
		float zNoise = perlinNoise(coords.x, coords.y, MAP::XYSCALE, zscale, MAP::XSEED, MAP::YSEED);
		return zNoise;
	}

	int getVertexPos(int x, int y) {
		int j = VerticesPos()[(y / grid_size * mapWidth / grid_size + x / grid_size)];
		return j;
	}

	glm::vec3 getVertex(int x, int y) {
		int j = getVertexPos(x, y);
		float zNoise = MapHeights()[j * 4];
		float x1 = (float)x;
		float y1 = (float)y + zNoise;
		float z1 = (float)y*(-1.f) + zNoise;
		return glm::vec3(x1, y1, z1);
	}

	std::array<Triangle, 6> getAdjacentTriangles(glm::ivec2 pos) {
		int gap = grid_size;
		std::array<Triangle, 6> triangles;
		triangles[0] = {
			getVertex(pos.x, pos.y), getVertex(pos.x - gap, pos.y),getVertex(pos.x - gap, pos.y + gap),
		};
		triangles[1] = {
			getVertex(pos.x, pos.y), getVertex(pos.x - gap, pos.y + gap), getVertex(pos.x, pos.y + gap)
		};
		triangles[2] = {
			getVertex(pos.x, pos.y), getVertex(pos.x, pos.y + gap), getVertex(pos.x + gap, pos.y)
		};
		triangles[3] = {
			getVertex(pos.x, pos.y), getVertex(pos.x + gap, pos.y), getVertex(pos.x + gap, pos.y - gap)
		};
		triangles[4] = {
			getVertex(pos.x, pos.y), getVertex(pos.x + gap, pos.y - gap),getVertex(pos.x, pos.y - gap),
		};
		triangles[5] = {
			getVertex(pos.x, pos.y), getVertex(pos.x, pos.y - gap), getVertex(pos.x - gap, pos.y)
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

			if (pos.x > grid_size && pos.x < mapWidth - grid_size && pos.y > grid_size && pos.y < mapHeight - grid_size) {

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
				MapHeights()[j * 4 + 3] = 1.f;
			}
		}
	}

	void updateAllNormals() {
		for (int i = 0; i < nVertices; i++) {
			float xCoord = MapVertices()[i * 4];
			float yCoord = MapVertices()[i * 4 + 1];
			glm::ivec2 pos = glm::ivec2(int(xCoord), int(yCoord));
			if (pos.x > grid_size && pos.x < mapWidth - grid_size && pos.y > grid_size && pos.y < mapHeight - grid_size) {
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
				MapHeights()[i * 4 + 3] = 1.f;
			}
		}
	}

	void generateRandomMap() {
		for (int i = 0; i < nVertices; i++) {
			float xCoord = MapVertices()[i * 4];
			float yCoord = MapVertices()[i * 4 + 1];

			float zNoise = generateNoise(glm::vec2(xCoord, yCoord), false);
			zNoise = smoothNoise(yCoord, zNoise);

			// update znoise
			MapHeights()[i * 4] = zNoise;
			MAP::MIN_Z = std::min(zNoise, MAP::MIN_Z);
			MAP::MAX_Z = std::max(zNoise, MAP::MAX_Z);
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

	void define_settlements() {

		srand((unsigned int)time(NULL));
		std::vector<float> townhallPos;
		int k = 0;
		int min = GAME::TOWNHALL_RADIUS + 100;
		int max_X = GAME::MAP_WIDTH - GAME::TOWNHALL_RADIUS - 100;
		int max_Y = GAME::MAP_HEIGHT - GAME::TOWNHALL_RADIUS - 100;
		float a, b;
		float d;
		bool c;
		for (int n = 0; n < (*PlayerList).size(); n++) {
			c = false;
			while (!c) {
				a = float(rand() % (max_X - min) + min);
				b = float(rand() % (max_Y - min) + min);

				if (n == 0) { // 1 PLAYER
					if (math::euclidean_distance(a, b, GAME::MAP_WIDTH / 2.0f, GAME::MAP_HEIGHT / 2.0f) > GAME::MAP_HEIGHT / 2.0f) {
						townhallPos.push_back(a);
						townhallPos.push_back(b);
						c = true;
					}
				}
				if (n == 1) { // 2 PLAYERS
					d = math::euclidean_distance(a, b, townhallPos[n - 1], townhallPos[n]);
					if (d > GAME::MAP_HEIGHT * 0.5) {
						townhallPos.push_back(a);
						townhallPos.push_back(b);
						c = true;
					}
				}
				if (n == 2 || n == 3) { // 3 & 4 PLAYERS
					bool c2 = true;
					for (int m = n - 1; m >= 0; m--) {
						d = math::euclidean_distance(a, b, townhallPos[m * 2], townhallPos[m * 2 + 1]);
						if (d <= GAME::MAP_HEIGHT * 0.5) {
							c2 = c2 * false;
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
						d = math::euclidean_distance(a, b, townhallPos[m * 2], townhallPos[m * 2 + 1]);
						if (d <= GAME::MAP_HEIGHT * 0.25) {
							c2 = c2 * false;
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
	}
}

/* ----- OBJECTS ----- */

namespace obj {

    AudioManager *Audio() { return &audioM; }
    BitmapFont *Text(){ return &txt; }
    BuildingSprite *BSprite(){ return &bsprite; }
    CursorImage *Cursor() { return &cursor; }
    EmptyCircle *ECircle() { return &eCircle; }
    EmptyRectangle *ERectangle() { return &eRect; }
    FilledRectangle *FRectangle() { return &fRect; }
    ImageSprite *Img() { return &img; }
    UnitSprite *USprite() { return &usprite; }
    Terrain *MapTerrain() { return &terrain; }
    Grid *MapGrid() { return &grid; }
    
    void init() {
        *Audio() = AudioManager();
        *Text() = BitmapFont();
        *BSprite() = BuildingSprite();
        *Cursor() = CursorImage();
        *ECircle() = EmptyCircle();
        *ERectangle() = EmptyRectangle();
        *FRectangle() = FilledRectangle();
        *Img() = ImageSprite();
        *USprite() = UnitSprite();
        *MapTerrain() = Terrain();
        *MapGrid() = Grid();
    }
    void compile() {
        Text()->compile();
        BSprite()->compile();
        ECircle()->compile();
        ERectangle()->compile();
        FRectangle()->compile();
        Cursor()->compile();
        Img()->compile();
        USprite()->compile();
        MapTerrain()->compile();
        MapGrid()->compile();
    }
    void create() {
        Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
        Cursor()->apply_projection_matrix(GLB::MENU_PROJECTION);

        //----

        Text()->create();
        Cursor()->create();
        ECircle()->create();
        ERectangle()->create();
        FRectangle()->create();
        BSprite()->create();	
        Img()->create();
        USprite()->create();

        //----

        MapTerrain()->create();
        MapGrid()->create();
    }

    void applyMenuMatrices() {
        Img()->apply_projection_matrix(GLB::MENU_PROJECTION);
        Img()->apply_view_matrix();
        ECircle()->apply_projection_matrix(GLB::MENU_PROJECTION);
        ECircle()->apply_view_matrix();
        ERectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
        ERectangle()->apply_view_matrix();
        FRectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
        FRectangle()->apply_view_matrix();
    }
    void applyGameMatrices(glm::mat4 *proj, glm::mat4 *view) {
        BSprite()->apply_projection_matrix(*proj);
        BSprite()->apply_view_matrix(*view);
        USprite()->apply_projection_matrix(*proj);
        USprite()->apply_view_matrix(*view);
        Img()->apply_projection_matrix(*proj);
        Img()->apply_view_matrix(*view);
        ECircle()->apply_projection_matrix(*proj);
        ECircle()->apply_view_matrix(*view);
        ERectangle()->apply_projection_matrix(*proj);
        ERectangle()->apply_view_matrix(*view);
        FRectangle()->apply_projection_matrix(*proj);
        FRectangle()->apply_view_matrix(*view);
        MapTerrain()->apply_projection_matrix(*proj);
        MapTerrain()->apply_view_matrix(*view);
        MapGrid()->apply_projection_matrix(*proj);
        MapGrid()->apply_view_matrix(*view);
    }
}
