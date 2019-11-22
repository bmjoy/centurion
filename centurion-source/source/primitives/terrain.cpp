#include <primitives>
#include <stb_image.h>
#include <game>
#include <surface>
#include <global>

using namespace glb;
using namespace game;

Terrain::Terrain(){
	vPath = "assets/shaders/terrain/vertex.glsl";
	fPath = "assets/shaders/terrain/fragment.glsl";
	terrainPathMap = map<string, string>();
}


void Terrain::create() {

	mapgen::init();

	// Read and store indices and vertices position information //

	readIndicesData(mapgen::Indices(), "assets/terrain/emptymap/indices");
	readVerticesData(mapgen::MapVertices(), "assets/terrain/emptymap/vertices");
	readVerticesPosData(mapgen::VerticesPos(), "assets/terrain/emptymap/vertices_pos");

	mapgen::reset_map();

	// Textures

	glUseProgram(shaderId);

	genBuffers();

	int k = 0;

	for (map<string, string>::iterator i = terrainPathMap.begin(); i != terrainPathMap.end(); i++) {

		texturesName.push_back(i->first);
		string texturePath = i->second;

		// load image
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


void Terrain::render(bool tracing) {
	glUseProgram(shaderId);

	// Wireframe
	glUniform1i(glGetUniformLocation(shaderId, "wireframe"), int(getBoolean("wireframe")));
	if (getBoolean("wireframe") && !tracing) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// Tracing 
	glUniform1i(glGetUniformLocation(shaderId, "tracing"), int(tracing));

	// Minimap
	glUniform1i(glGetUniformLocation(shaderId, "minimap"), int(game::gameMinimapStatus));
	
	//-------------------------------------//
	/*               DRAWING               */
	//-------------------------------------//
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	

	/* TRACING */

	if (tracing) {
		glUniform1f(glGetUniformLocation(shaderId, "minZ"), mapgen::minZ);
		glUniform1f(glGetUniformLocation(shaderId, "maxZ"), mapgen::maxZ);

		/* Draw */	
		glDrawElements(GL_TRIANGLES, mapgen::nIndices, GL_UNSIGNED_INT, 0);
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
		float scaleTexX = (float)mapWidth / width;    // to fix the texture scale
		float scaleTexY = (float)mapHeight / height;
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
		glDrawElements(GL_TRIANGLES, mapgen::nIndices, GL_UNSIGNED_INT, 0);
		glDisable(GL_DEPTH_TEST);
	}
		
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Terrain::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*mapgen::Indices()) * mapgen::nIndices, mapgen::Indices(), GL_STATIC_DRAW);
	/* Vertices VBO */
	glGenBuffers(1, &VerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapVertices()) * mapgen::nVertices * 4, mapgen::MapVertices(), GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapHeights()) * mapgen::nVertices * 4, mapgen::MapHeights(), GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapTextures()) * mapgen::nVertices, mapgen::MapTextures(), GL_STATIC_DRAW);
	// textureType
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::updateHeightsBuffer() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, HeightsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapHeights()) * mapgen::nVertices * 4, mapgen::MapHeights(), GL_STATIC_DRAW);
	// zNoise
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	// xNorm, yNorm, zNorm
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 1));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::updateTextureBuffer() {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, TexturesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapTextures()) * mapgen::nVertices, mapgen::MapTextures(), GL_STATIC_DRAW);
	// textureType
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Terrain::~Terrain()
{
}
