#include "terrain.h"
#include "stb_image.h" 

Terrain::Terrain(){
	vPath = "assets/shaders/terrain/vertex.glsl";
	fPath = "assets/shaders/terrain/fragment.glsl";
	path_grass = "assets/terrain/textures/grass1.png";
	path_road = "assets/terrain/textures/road1.png";
	path_rock = "assets/terrain/textures/rock1.png";
	plane_path = "assets/terrain/plane.obj";

	verticesPosMap = std::map<std::string, int>();
	verticesPosMap["x"] = 0;
	verticesPosMap["y"] = 1;
	verticesPosMap["z"] = 2;
	verticesPosMap["zNoise"] = 3;
	verticesPosMap["xTex"] = 4;
	verticesPosMap["yTex"] = 5;
	verticesPosMap["xNorm"] = 6;
	verticesPosMap["yNorm"] = 7;
	verticesPosMap["zNorm"] = 8;
	verticesPosMap["terrain"] = 9;
}


void Terrain::create() {
	nIndices = 219024;
	nVertices = 368950;

	mapgen::init();

	// Read and store indices and vertices position information //

	{
		std::fstream fin;
		fin.open("assets/terrain/emptymap/indices");
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			mapgen::Indices()[i] = (unsigned int)std::stoi(number);
			i++;
		}
	}

	{
		std::fstream fin;
		fin.open("assets/terrain/emptymap/vertices_position");
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			mapgen::VerticesPos()[i] = std::stoi(number);
			i++;
		}
	}

	{
		std::fstream fin;
		fin.open("assets/terrain/emptymap/vertices");
		std::string line, number;
		std::getline(fin, line);
		std::stringstream s(line);
		int i = 0;
		while (std::getline(s, number, ',')) {
			mapgen::EmptyMapVertices()[i] = std::stof(number);
			i++;
		}
	}

	mapgen::reset_map();

	genBuffers();
	texture();
}


void Terrain::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*mapgen::Indices()) * nIndices, mapgen::Indices(), GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapVertices()) * nVertices, mapgen::MapVertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 4));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 9));
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::updateBuffers(float x, float y, std::string type, float q) {

	int k = verticesPosMap[type];
	int j = (int)((int)y / 128 * GAME::MAP_WIDTH / 128 + (int)x / 128);

	mapgen::MapVertices()[mapgen::VerticesPos()[j] * 10 + k] = q;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(*mapgen::MapVertices()) * nVertices, mapgen::MapVertices(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 4));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float) * 9));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Terrain::texture() {
	/* Texture: GRASS */
	textureIdList.push_back(0);
	textureInfoList.push_back(glm::ivec3(0, 0, 0));
	unsigned char *data = stbi_load(path_grass, &textureInfoList[0].x, &textureInfoList[0].y, &textureInfoList[0].z, 0);
	if (!data) { std::cout << "Failed to load texture" << std::endl; }
	glGenTextures(1, &textureIdList[0]);
	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[0].x, textureInfoList[0].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	/* Texture 3: ROAD */
	textureIdList.push_back(0);
	textureInfoList.push_back(glm::ivec3(0, 0, 0));
	data = stbi_load(path_road, &textureInfoList[1].x, &textureInfoList[1].y, &textureInfoList[1].z, 0);
	if (!data) { std::cout << "Failed to load texture" << std::endl; }
	glGenTextures(1, &textureIdList[1]);
	glBindTexture(GL_TEXTURE_2D, textureIdList[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[1].x, textureInfoList[1].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}



void Terrain::render(bool tracing) {
	glUseProgram(shaderId);

	// Wireframe
	glUniform1i(glGetUniformLocation(shaderId, "wireframe"), int(GLB::WIREFRAME));
	if (GLB::WIREFRAME && !tracing) {
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
		glUniform1f(glGetUniformLocation(shaderId, "minZ"), MAP::MIN_Z);
		glUniform1f(glGetUniformLocation(shaderId, "maxZ"), MAP::MAX_Z);

		/* Draw */	
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
	}

	/* NORMAL RENDERING */

	else {
		// Light
		GLuint uAmbientIntensity = 0, uAmbientColor = 0, uDiffuseIntensity = 0, uDiffuseDirection = 0;
		uAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.ambientIntensity");
		uAmbientColor = glGetUniformLocation(shaderId, "directionalLight.colour");
		uDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.diffuseIntensity");
		uDiffuseDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
		light.use_light(uAmbientIntensity, uAmbientColor, uDiffuseIntensity, uDiffuseDirection);

		// Texture
		float scaleTexX = (float)GAME::MAP_WIDTH / (float)textureInfoList[1].x;    // to fix the texture scale
		float scaleTexY = (float)GAME::MAP_HEIGHT / (float)textureInfoList[1].y;
		glUniform1f(glGetUniformLocation(shaderId, "scaleTextX"), scaleTexX);
		glUniform1f(glGetUniformLocation(shaderId, "scaleTextY"), scaleTexY);
		glUniform1i(glGetUniformLocation(shaderId, "grass1"), 0);
		glUniform1i(glGetUniformLocation(shaderId, "road1"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureIdList[1]);

		/* Draw */
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);
	}
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


Terrain::~Terrain()
{
}
