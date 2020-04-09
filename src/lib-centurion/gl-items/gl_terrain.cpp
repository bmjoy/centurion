#include "gl_terrain.h"

#include <stb_image.h>
#include <game/strategy.h>
#include <game/map.h>
#include <logger.h>
#include <engine.h>

#include <tinyxml2.h>
#include <file_manager.h>
#include <game/editor.h>

using namespace std;
using namespace glm;

glTerrain::glTerrain() {
	vPath = "assets/shaders/terrain/vertex.glsl";
	fPath = "assets/shaders/terrain/fragment.glsl";
}

void glTerrain::create() {
	// Read and store indices and vertices position information //

	ReadIndicesData();
	ReadVerticesData();
	ReadVerticesPosData();

	Game::Mapgen::ResetTexturesAndHeights();

	// Textures
	glUseProgram(shaderId);

	genBuffers();

	int k = 0;

	string path = Folders::GAME + "assets\\terrain\\terrains.xml";
	tinyxml2::XMLDocument xmlFile;
	xmlFile.LoadFile(path.c_str());

	tinyxml2::XMLElement *_terrains = xmlFile.FirstChildElement("terrains");
	for (tinyxml2::XMLElement* _terr = _terrains->FirstChildElement(); _terr != NULL; _terr = _terr->NextSiblingElement())
	{
		//Terrain data
		terrainTexture tData = terrainTexture();
		tData.id = _terr->IntAttribute("id");
		tData.name = _terr->Attribute("name");
		tData.zones = vector<string>();
		tData.frequencies = vector<float>();
		tinyxml2::XMLElement *_zones = _terr->FirstChildElement("zoneArray");
		for (tinyxml2::XMLElement* _zone = _zones->FirstChildElement(); _zone != NULL; _zone = _zone->NextSiblingElement())
		{
			tData.zones.push_back(string(_zone->Attribute("name")));
			tData.frequencies.push_back(stof(string(_zone->Attribute("frequency"))));

			Game::Mapgen::zonesMap[string(_zone->Attribute("name"))].push_back(tData.name);
		}
		Game::Mapgen::terrainsMap[tData.name] = tData;
		texturesName.push_back(tData.name);

		// send data to editor terrain tree 
		std::string editorTreeListName = _terr->Attribute("editorTreeList");
		Game::Editor::AddEditorTerrainTreeElement(editorTreeListName, tData.name);

		// load image
		string texturePath = Folders::GAME + "assets\\terrain\\Textures\\" + tData.name + ".png";
		textureIdList.push_back(0);
		textureInfoList.push_back(glm::ivec3(0, 0, 0));
		unsigned char *data = stbi_load(texturePath.c_str(), &textureInfoList[k].x, &textureInfoList[k].y, &textureInfoList[k].z, 0);
		if (!data) { std::cout << "Failed to load texture" << std::endl; }

		width = float(textureInfoList[k].x); height = float(textureInfoList[k].y);

		glGenTextures(1, &textureIdList[k]);
		glBindTexture(GL_TEXTURE_2D, textureIdList[k]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[k].x, textureInfoList[k].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		k++;
	}
}


void glTerrain::render(bool tracing) {
	glUseProgram(shaderId);

	// Wireframe
	glUniform1i(glGetUniformLocation(shaderId, "wireframe"), int(Game::Map::Wireframe));
	if (Game::Map::Wireframe && !tracing) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Tracing 
	glUniform1i(glGetUniformLocation(shaderId, "tracing"), int(tracing));

	//-------------------------------------//
	/*               DRAWING               */
	//-------------------------------------//

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	/* TRACING */

	if (tracing) {
		glUniform1f(glGetUniformLocation(shaderId, "minZ"), Game::Mapgen::GetMinZ());
		glUniform1f(glGetUniformLocation(shaderId, "maxZ"), Game::Mapgen::GetMaxZ());

		/* Draw */
		glDrawElements(GL_TRIANGLES, Game::Mapgen::GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
	}

	/* NORMAL RENDERING */


	else {
		glEnable(GL_DEPTH_TEST);
		// Light
		GLuint uAmbientIntensity = 0, uAmbientColor = 0, uDiffuseIntensity = 0, uDiffuseDirection = 0;
		uAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.ambientIntensity");
		uAmbientColor = glGetUniformLocation(shaderId, "directionalLight.colour");
		uDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.diffuseIntensity");
		uDiffuseDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
		light.use_light((GLfloat)uAmbientIntensity, (GLfloat)uAmbientColor, (GLfloat)uDiffuseIntensity, (GLfloat)uDiffuseDirection);

		// Texture
		float scaleTexX = (float)MEDIUM_MAP_WIDTH / width;    // to fix the texture scale
		float scaleTexY = (float)MEDIUM_MAP_HEIGHT / height;
		glUniform1f(glGetUniformLocation(shaderId, "scaleTextX"), scaleTexX);
		glUniform1f(glGetUniformLocation(shaderId, "scaleTextY"), scaleTexY);
		glUniform1i(glGetUniformLocation(shaderId, "nTerrains"), int(texturesName.size()));

		for (int j = 0; j < texturesName.size(); j++) {
			string sample = "sampleTex[" + to_string(j) + "]";
			glUniform1i(glGetUniformLocation(shaderId, sample.c_str()), j);
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, textureIdList[j]);
		}

		/* Draw */
		glDrawElements(GL_TRIANGLES, Game::Mapgen::GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
		glDisable(GL_DEPTH_TEST);
	}


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void glTerrain::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*Game::Mapgen::Indices()) * Game::Mapgen::GetNumberOfIndices(), Game::Mapgen::Indices(), GL_STATIC_DRAW);
	/* Vertices VBO */
	glGenBuffers(1, &VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Game::Mapgen::MapVertices()) * Game::Mapgen::GetNumberOfVertices() * 4, Game::Mapgen::MapVertices(), GL_STATIC_DRAW);
	// xCoord, yCoord 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// xTexCoord, yTexCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/* Heights VBO */
	glGenBuffers(1, &HeightsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, HeightsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Game::Mapgen::MapHeights()) * Game::Mapgen::GetNumberOfVertices() * 4, Game::Mapgen::MapHeights(), GL_STATIC_DRAW);
	// zNoise
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// xNorm, yNorm, zNorm
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 1));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	/* Textures VBO */
	glGenBuffers(1, &TexturesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TexturesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Game::Mapgen::MapTextures()) * Game::Mapgen::GetNumberOfVertices(), Game::Mapgen::MapTextures(), GL_STATIC_DRAW);
	// textureType
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void glTerrain::ReadIndicesData(void)
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
		Engine::GameClose();
	}

}

void glTerrain::ReadVerticesData(void)
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
		Engine::GameClose();
	}
}

void glTerrain::ReadVerticesPosData(void)
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
		Engine::GameClose();
	}
}

void glTerrain::updateHeightsBuffer() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, HeightsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Game::Mapgen::MapHeights()) * Game::Mapgen::GetNumberOfVertices() * 4, Game::Mapgen::MapHeights(), GL_STATIC_DRAW);
	// zNoise
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// xNorm, yNorm, zNorm
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 1));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void glTerrain::updateTextureBuffer() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TexturesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*Game::Mapgen::MapTextures()) * Game::Mapgen::GetNumberOfVertices(), Game::Mapgen::MapTextures(), GL_STATIC_DRAW);
	// textureType
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glTerrain::~glTerrain()
{
}
