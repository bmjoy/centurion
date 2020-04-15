/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>

#ifndef MAX_NUMBER_OF_TEXTURES
#define MAX_NUMBER_OF_TEXTURES 50
#endif // !MAX_NUMBER_OF_TEXTURES


namespace Math { struct Triangle; }
struct terrainTexture;

namespace Game
{
	namespace Mapgen
	{
		class TerrainTexture
		{
		public:

			struct TerrainZone
			{
				std::string name;
				float frequency;
				int zoneId;
			};

			TerrainTexture();
			TerrainTexture(int _id, std::string _name, std::string _path, std::vector<TerrainZone> _zones, std::string _editortree);
			std::string GetName(void) { return name; }
			std::string GetPath(void) { return path; }
			std::string GetEditorTree(void) { return editortree; }
			int GetId(void) { return terrainId; }
			~TerrainTexture();
		private:
			std::vector<TerrainZone> zones;
			std::string name;
			std::string editortree;
			std::string path;
			//float width, height; TO DO
			int terrainId;
		};

		/// <summary>
		/// This function prepares the terrain arrays and textures.
		/// </summary>
		void InitializeTerrain(void);

		/// <summary>
		/// This function reads the file XML that contains the texture information
		/// </summary>
		void ReadTexturesXml(void);

		/// <summary>
		/// This function adds an element of type TerrainTexture to the array of textures of size MAX_NUMBER_OF_TEXTURES
		/// </summary>
		/// <param name="id">The texture id</param>
		/// <param name="TerrainTexture">An element of type TerrainTexture</param>
		void AddTerrainTexture(const unsigned int id, TerrainTexture tt);

		/// <summary>
		/// This function returns a pointer to the required terrain texture
		/// </summary>
		/// <param name="id">The texture id</param>
		Mapgen::TerrainTexture* GetTerrainTexturePtrById(const unsigned int id);

		/// <summary>
		/// This function returns a pointer to the required terrain texture
		/// </summary>
		/// <param name="_name">The texture name. It supports only const string values</param>
		Mapgen::TerrainTexture* GetTerrainTexturePtrByName(const std::string _name);

		/// <summary>
		/// This function read the "indices" file that contains information about the indices of plane vertices
		/// </summary>
		void ReadIndicesData(void);

		/// <summary>
		/// This function read the "vertices" file that contains information about plane vertices
		/// </summary>
		void ReadVerticesData(void);

		/// <summary>
		/// This function read the "vertices_pos" file that contains information about the position of plane vertices
		/// </summary>
		void ReadVerticesPosData(void);

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

		/// <summary>
		/// This function resets the Heights and Textures arrays to empty map standard values
		/// </summary>
		/// <param name="type">The texture type. It must be a float value, equal to 1.f, 2.f, ... and represents the terrain ID in float format.</param>
		/// <param name="x">The point X-position, expressed with pixel-map-coordinates, where apply the terrain change</param>
		/// <param name="y">The point Y-position, expressed with pixel-map-coordinates, where apply the terrain change</param>
		void ChangeTerrainTexture(const float type, const float x, const float y);

		/// <summary>
		/// This function generate a surface with pseudo-random noise
		/// </summary>
		void GenerateRandomHeights(void);

		/// <summary>
		/// This function reset all the vertices normals to obtain the right shadows
		/// </summary>
		void UpdateAllNormals(void);



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
		void updateNormal(int x, int y);
		
		//
		//	MAP GENERATION FUNCTIONS
		//

		
		float generateNoise(glm::vec2 coords, std::string type);
		void define_buildings_location(int num_players, int num_outposts, std::vector<glm::vec2>* outpostslocation, std::vector<glm::vec2>* townhallslocation);
		void defineTerrainZones();
	};
};