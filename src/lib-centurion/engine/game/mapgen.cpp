#include "mapgen.h"
#include "editor.h"

#include <random>
#include <ctime>

#include <maths.hpp>
#include <classes/building.h>
#include <gl_terrain.h>
#include <logger.h>
#include <file_manager.h>


using namespace std;
using namespace glm;

namespace Game
{

	// Mapgen elements 
	namespace Mapgen
	{
		// Mapgen private elements
		namespace
		{
			const int TRIANGLE_WIDTH = 128;
			const int NUMBER_OF_INDICES = 228480;
			const int NUMBER_OF_VERTICES = 38479;
			const int MAP_MESH_WIDTH = 30464;  // 29952 + 128*4
			const int MAP_MESH_HEIGHT = 20480; // 19968 + 128*4

			float map_vertices[NUMBER_OF_VERTICES * 4] = { 0.f };
			float map_textures[NUMBER_OF_VERTICES] = { 1.f };
			float map_heights[NUMBER_OF_VERTICES * 4] = { 0.f };
			unsigned int indices[NUMBER_OF_INDICES] = { 0 };
			int vertices_pos[NUMBER_OF_VERTICES] = { 0 };

			float MIN_Z_REACHED = 0.f;
			float MAX_Z_REACHED = 0.f;

			array<TerrainTexture, MAX_NUMBER_OF_TEXTURES> TERRAIN_TEXTURES = array<TerrainTexture, MAX_NUMBER_OF_TEXTURES>();
			unordered_map<string, TerrainTexture*> TERRAIN_TEXTURES_MAP = unordered_map<string, TerrainTexture*>();
		};

		void InitializeTerrain(void)
		{
			Mapgen::ReadIndicesData();
			Mapgen::ReadVerticesData();
			Mapgen::ReadVerticesPosData();
			Mapgen::ResetTexturesAndHeights();
			Mapgen::ReadTexturesXml();
		}		

		int GetTriangleWidth(void)
		{
			return TRIANGLE_WIDTH;
		}

		int GetNumberOfIndices(void)
		{
			return NUMBER_OF_INDICES;
		}

		int GetNumberOfVertices(void)
		{
			return NUMBER_OF_VERTICES;
		}

		int GetMaxZ(void)
		{
			return MAX_Z_REACHED;
		}

		int GetMinZ(void)
		{
			return MIN_Z_REACHED;
		}

		int GetMapMeshHeight(void)
		{
			return MAP_MESH_HEIGHT;
		}

		float* MapVertices()
		{
			return map_vertices;
		}

		float* MapTextures()
		{
			return map_textures;
		}

		float* MapHeights()
		{
			return map_heights;
		}

		unsigned int* Indices()
		{
			return indices;
		}

		int* VerticesPos()
		{
			return vertices_pos;
		}

		void ResetTexturesAndHeights(void) {
			for (int i = 0; i < NUMBER_OF_VERTICES; i++) {

				/* reset textures */
				MapTextures()[i] = 1.f;

				/* reset noise and normals */
				MapHeights()[i * 4] = 0.f;
				MapHeights()[i * 4 + 1] = 0.f;
				MapHeights()[i * 4 + 2] = 0.f;
				MapHeights()[i * 4 + 3] = 0.707107f;
			}
		}

		void ChangeTerrainTexture(const float type, const float x, const float y)
		{
			int ix = round(x / TRIANGLE_WIDTH) * TRIANGLE_WIDTH + TRIANGLE_WIDTH * 2;
			int iy = round(y / TRIANGLE_WIDTH) * TRIANGLE_WIDTH + TRIANGLE_WIDTH * 2;

			int j = Game::Mapgen::getVertexPos(ix, iy);

			if (Game::Mapgen::MapTextures()[j] != type)
			{
				Game::Mapgen::MapTextures()[j] = type;
				GLItems::MapTerrain()->updateTextureBuffer();
			}
		}

		//-------------

		float generateNoise(vec2 coords, string type) {

			float XYSCALE = 0.f;
			float ZSCALE = 0.f;
			float XSEED = 0.f;
			float YSEED = 0.f;

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

			float zNoise = Math::perlin_noise(coords.x, coords.y, XYSCALE, ZSCALE, XSEED, YSEED, MAP_MESH_WIDTH, MAP_MESH_HEIGHT);
			return zNoise;
		}

		int getVertexPos(int x, int y) {
			int j = VerticesPos()[(y / TRIANGLE_WIDTH * MAP_MESH_WIDTH / TRIANGLE_WIDTH + x / TRIANGLE_WIDTH)];
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
			float z1 = (float)y * (-1.f) + zNoise;
			return glm::vec3(x1, y1, z1);
		}

		std::array<Math::Triangle, 6> GetAdjacentTriangles(glm::ivec2 pos) {
			int gap = TRIANGLE_WIDTH;
			std::array<Math::Triangle, 6> triangles;
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

		void updateNormal(int x, int y) {
			std::vector<glm::ivec2> vertices = Math::get_adjacent_vertices(glm::ivec2(x, y), TRIANGLE_WIDTH);
			vertices.push_back(glm::ivec2(x, y));

			for (int i = 0; i < 7; i++) {
				glm::ivec2 pos = glm::ivec2(vertices[i].x, vertices[i].y);
				int j = getVertexPos(pos.x, pos.y);

				if (pos.x > TRIANGLE_WIDTH && pos.x < MAP_MESH_WIDTH - TRIANGLE_WIDTH && pos.y > TRIANGLE_WIDTH && pos.y < MAP_MESH_HEIGHT - TRIANGLE_WIDTH) {

					std::array<Math::Triangle, 6> adjacentTriangles = GetAdjacentTriangles(pos);
					glm::vec3 sum(0.f);
					for (int k = 0; k < 6; k++) {
						glm::vec3 normal = Math::calculate_triangle_normal(adjacentTriangles[k]);
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

		void UpdateAllNormals() {
			for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
				float xCoord = MapVertices()[i * 4];
				float yCoord = MapVertices()[i * 4 + 1];
				glm::ivec2 pos = glm::ivec2(int(xCoord), int(yCoord));
				if (pos.x > TRIANGLE_WIDTH && pos.x < MAP_MESH_WIDTH - TRIANGLE_WIDTH && pos.y > TRIANGLE_WIDTH && pos.y < MAP_MESH_HEIGHT - TRIANGLE_WIDTH) {
					std::array<Math::Triangle, 6> adjacentTriangles = GetAdjacentTriangles(pos);
					glm::vec3 sum(0.f);

					for (int k = 0; k < 6; k++) {
						glm::vec3 normal = Math::calculate_triangle_normal(adjacentTriangles[k]);
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

		void GenerateRandomHeights() {
			//random_device rd;
			//mt19937 gen(rd());
			//uniform_real_distribution<float> distribution(0, 3);

			for (int i = 0; i < NUMBER_OF_VERTICES; i++) {
				float xCoord = MapVertices()[i * 4];
				float yCoord = MapVertices()[i * 4 + 1];

				float zNoise = generateNoise(glm::vec2(xCoord, yCoord), "height");
				zNoise = Math::smooth_noise(yCoord, zNoise, MAP_MESH_HEIGHT);

				//float zTex = generateNoise(glm::vec2(xCoord, yCoord), "texture");
				//zTex = smoothNoise(yCoord, zTex);

				// update znoise
				MapHeights()[i * 4] = zNoise;
				MIN_Z_REACHED = std::min(zNoise, MIN_Z_REACHED);
				MAX_Z_REACHED = std::max(zNoise, MAX_Z_REACHED);

				// terrain
				//float p = distribution(gen);
				//if (p <= 1.5) MapTextures()[i] = 1.f;
				//if (p > 1.5 && p <= 2.5) MapTextures()[i] = 2.f;
				//if (p > 2.5 && p <= 3) MapTextures()[i] = 4.f;


				// update texture
				//if (zTex > 1000)
					//MapTextures()[i] = 10.f;
			}
			UpdateAllNormals();
		}

		float getNoiseEstimate(float x, float y) {

			int gap = TRIANGLE_WIDTH;

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
			float dx = x / TRIANGLE_WIDTH - x0 / TRIANGLE_WIDTH;
			float dy = y / TRIANGLE_WIDTH - y0 / TRIANGLE_WIDTH;

			// values 
			float v0 = z0 * (1 - dx) + z1 * dx; // bottom
			float v1 = z1 * (1 - dy) + z2 * dy; // right
			float v2 = z3 * (1 - dx) + z2 * dx; // top
			float v3 = z0 * (1 - dy) + z3 * dy; // left

			// average
			float zHat = (v0 * (1 - dy) + v1 * dx + v2 * dy + v3 * (1 - dx)) / 2.f;

			return zHat;
		}

		void define_buildings_location(int num_players, int num_outposts, vector<vec2>* outpostslocation, vector<vec2>* townhallslocation) {
			default_random_engine generator;
			srand((unsigned int)time(NULL));
			vector<vec2> townhallPos(num_players, vec2(0));
			vector<vec2> outpostsPos(num_outposts, vec2(0));

			/* locations grid definition */
			vector<vec2> locGrid;
			int min = TOWNHALL_RADIUS + TRIANGLE_WIDTH;
			int max_X = MAP_MESH_WIDTH - TOWNHALL_RADIUS - TRIANGLE_WIDTH;
			int max_Y = MAP_MESH_HEIGHT - TOWNHALL_RADIUS - TRIANGLE_WIDTH;
			for (int iHoriz = 0; iHoriz < MAP_MESH_WIDTH - TRIANGLE_WIDTH * 4; iHoriz += TRIANGLE_WIDTH) {
				for (int iVert = 0; iVert < MAP_MESH_HEIGHT - TRIANGLE_WIDTH * 4; iVert += TRIANGLE_WIDTH) {
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

					if ((a >= MAP_MESH_WIDTH - TOWNHALL_RADIUS * 1.1) || (a <= TOWNHALL_RADIUS * 1.1) ||
						(b >= MAP_MESH_HEIGHT - TOWNHALL_RADIUS * 1.1) || (b <= TOWNHALL_RADIUS * 1.1)) continue;

					if (n == 0) { // 1 PLAYER
						if (Math::euclidean_distance(a, b, MAP_MESH_WIDTH / 2.0f, MAP_MESH_HEIGHT / 2.0f) > MAP_MESH_HEIGHT / 2.0f) {
							townhallPos[n].x = a;
							townhallPos[n].y = b;
							c = true;
						}
					}
					if (n == 1) { // 2 PLAYERS
						float d = Math::euclidean_distance(a, b, townhallPos[0].x, townhallPos[0].y);
						if (d > MAP_MESH_HEIGHT * 0.5) {
							townhallPos[n].x = a;
							townhallPos[n].y = b;
							c = true;
						}
					}
					if (n == 2 || n == 3) { // 3 & 4 PLAYERS
						bool c2 = true;
						for (int m = n - 1; m >= 0; m--) {
							float d = Math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y);
							if (d <= MAP_MESH_HEIGHT * 0.5) {
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
							float d = Math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y);
							if (d <= MAP_MESH_HEIGHT * 0.25) {
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
					float d = Math::euclidean_distance(locGrid[i].x, locGrid[i].y, townhallPos[j].x, townhallPos[j].y);
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

					if ((a >= MAP_MESH_WIDTH - OUTPOST_RADIUS * 1.25f) || (a <= OUTPOST_RADIUS * 1.25f) ||
						(b >= MAP_MESH_HEIGHT - OUTPOST_RADIUS * 1.25f) || (b <= OUTPOST_RADIUS * 1.25f)) continue;

					bool c2 = true;
					for (int m = 0; m < townhallPos.size(); m++) {
						if (Math::euclidean_distance(a, b, townhallPos[m].x, townhallPos[m].y) < TOWNHALL_RADIUS + 2 * OUTPOST_RADIUS) {
							c2 = c2 * false;
						}
					}
					for (int m = n - 1; m >= 0; m--) {
						if (Math::euclidean_distance(a, b, outpostsPos[m].x, outpostsPos[m].y) <= OUTPOST_RADIUS * 4) {
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


			/*for (int i = 0; i < NUMBER_OF_VERTICES; i++) {

				string zoneType = "none";

				float d = MAP_MESH_WIDTH * 2;
				vec2 p = getVertexCoords(i) - 2.f * TRIANGLE_WIDTH;

				vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();
				for (int i = 0; i < listOfIndipBuildings.size(); i++) {
					Building* b = listOfIndipBuildings[i];
					if (b->GetClassName().substr(1) != "townhall") continue;

					float rnoise = distribution(gen);

					float dist = Math::euclidean_distance(p.x, p.y, b->GetPosition().x, b->GetPosition().y);
					dist += rnoise;
					if (dist <= d) {
						d = dist;
						string race = b->GetRaceName();
						if (dist - rnoise > TOWNHALL_RADIUS - TRIANGLE_WIDTH) zoneType = Game::GetRace(race)->getEnvironmentalZone();
						else zoneType = "none";
					}
				}

				if (zoneType != "none") {
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
					uniform_int_distribution<int> distribution(0, int(weightedTerrainsId.size() - 1));
					int j = distribution(gen);
					MapTextures()[i] = weightedTerrainsId[j];
				}
			}*/
			// update texture buffer
			GLItems::MapTerrain()->updateTextureBuffer();
		}

		void ReadTexturesXml(void)
		{
			try
			{
				string path = Folders::GAME + "assets\\terrain\\terrains.xml";
				tinyxml2::XMLDocument xmlFile;
				xmlFile.LoadFile(path.c_str());

				tinyxml2::XMLElement* _terrains = xmlFile.FirstChildElement("terrains");
				for (tinyxml2::XMLElement* _terr = _terrains->FirstChildElement(); _terr != NULL; _terr = _terr->NextSiblingElement())
				{
					int _id = _terr->IntAttribute("id");
					string _name = _terr->Attribute("name");
					string _path = _terr->Attribute("path");
					string _editorTree = _terr->Attribute("editorTreeList");

					vector<TerrainTexture::TerrainZone> _zonesList = vector<TerrainTexture::TerrainZone>();

					tinyxml2::XMLElement* _zones = _terr->FirstChildElement("zoneArray");
					for (tinyxml2::XMLElement* _zone = _zones->FirstChildElement(); _zone != NULL; _zone = _zone->NextSiblingElement())
					{
						TerrainTexture::TerrainZone terrainZone = TerrainTexture::TerrainZone();
						terrainZone.frequency = _zone->FloatAttribute("frequency");
						terrainZone.name = _zone->Attribute("name");
						terrainZone.zoneId = _zone->IntAttribute("id");
						_zonesList.push_back(terrainZone);
					}

					TerrainTexture tt = TerrainTexture(_id, _name, _path, _zonesList, _editorTree);
					Mapgen::AddTerrainTexture(_id, tt);
				}
			}
			catch (const std::exception&)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the textures XML", "Error", "Game::Mapgen", "", "ReadTexturesXml");
				Logger::Error(msg);
				throw;
			}


		}

		void AddTerrainTexture(const unsigned int id, TerrainTexture tt)
		{
			if (id < 0 || id >= MAX_NUMBER_OF_TEXTURES) return;
			TERRAIN_TEXTURES[id] = tt;
			TERRAIN_TEXTURES_MAP[tt.GetName()] = &(TERRAIN_TEXTURES[id]);
			Editor::AddEditorTerrainTreeElement(tt.GetEditorTree(), tt.GetName());
			GLItems::MapTerrain()->AddTerrainTexturePath(tt.GetPath());
		}

		Mapgen::TerrainTexture* GetTerrainTexturePtrById(const unsigned int id)
		{
			if (id < 0 || id >= MAX_NUMBER_OF_TEXTURES) return nullptr;
			return &(TERRAIN_TEXTURES[id]);
		}

		Mapgen::TerrainTexture* GetTerrainTexturePtrByName(const std::string _name)
		{
			return TERRAIN_TEXTURES_MAP[_name];
		}

		void ReadIndicesData(void)
		{
			try
			{
				fstream fin;
				fin.open("assets/terrain/emptymap/indices");
				string line, number;
				getline(fin, line);
				stringstream s(line);
				int i = 0;
				while (getline(s, number, ',')) {
					Game::Mapgen::Indices()[i] = (unsigned int)stoi(number);
					i++;
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the indices data", "Error", "", "Terrain", "ReadIndicesData");
				Logger::Error(msg);
				throw;
			}
		}

		void ReadVerticesData(void)
		{
			try
			{
				fstream fin;
				fin.open("assets/terrain/emptymap/vertices");
				string line, number;
				getline(fin, line);
				stringstream s(line);
				int i = 0;
				while (getline(s, number, ',')) {
					Game::Mapgen::MapVertices()[i] = stof(number);
					i++;
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the vertices data", "Error", "", "Terrain", "ReadVerticesData");
				Logger::Error(msg);
				throw;
			}
		}

		void ReadVerticesPosData(void)
		{
			try {
				fstream fin;
				fin.open("assets/terrain/emptymap/vertices_pos");
				string line, number;
				getline(fin, line);
				stringstream s(line);
				int i = 0;
				while (getline(s, number, ',')) {
					Game::Mapgen::VerticesPos()[i] = stoi(number);
					i++;
				}
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the vertices pos data", "Error", "", "Terrain", "ReadVerticesPosData");
				Logger::Error(msg);
				throw;
			}
		}

		TerrainTexture::TerrainTexture() {}

		TerrainTexture::TerrainTexture(int _id, std::string _name, std::string _path, std::vector<TerrainTexture::TerrainZone> _zones, std::string _editortree)
		{
			terrainId = _id;
			name = _name;
			path = Folders::GAME + _path;
			zones = _zones;
			editortree = _editortree;
		}

		TerrainTexture::~TerrainTexture() {}
	};
};
