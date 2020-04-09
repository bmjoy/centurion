/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>

namespace Math { struct Triangle; }
struct terrainTexture;

namespace Game
{
	namespace Mapgen
	{
		extern std::map<std::string, std::vector<std::string>> zonesMap;
		extern std::map<std::string, terrainTexture> terrainsMap;

		/// <summary>
		/// This function returns the width of the triangles that compose the map mesh.
		/// </summary>
		int GetTriangleWidth(void);

		/// <summary>
		/// This function returns the number of indices of the entire mesh.
		/// </summary>
		int GetNumberOfIndices(void);

		/// <summary>
		/// This function returns the number of indices of the entire mesh.
		/// </summary>
		int GetNumberOfVertices(void);

		/// <summary>
		/// This function returns the maximum Z generated by the noise generator
		/// </summary>
		int GetMaxZ(void);
		
		/// <summary>
		/// This function returns the minimum Z generated by the noise generator
		/// </summary>
		int GetMinZ(void);

		/// <summary>
		/// This function returns the height (y-size) of the map mesh.
		/// </summary>
		int GetMapMeshHeight(void);

		/// <summary>
		/// This function resets the Heights and Textures arrays to empty map standard values
		/// </summary>
		void ResetTexturesAndHeights(void);

		float* MapVertices();
		float* MapTextures();
		float* MapHeights();
		unsigned int* Indices();
		int* VerticesPos();

		std::array<Math::Triangle, 6> GetAdjacentTriangles(glm::ivec2 pos);
		float getNoiseEstimate(float x, float y);
		int getVertexPos(int x, int y);
		glm::vec2 getVertexCoords(int j);
		glm::vec3 getVertex(int x, int y, bool isNormal);
		void updateAllNormals();
		void updateNormal(int x, int y);
		
		//
		//	MAP GENERATION FUNCTIONS
		//

		void generateRandomMap();
		float generateNoise(glm::vec2 coords, std::string type);
		void define_buildings_location(int num_players, int num_outposts, std::vector<glm::vec2>* outpostslocation, std::vector<glm::vec2>* townhallslocation);
		void defineTerrainZones();
	};
};