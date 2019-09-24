#include "terrain.h"

#include "stb_image.h" 

Terrain::Terrain(){
	vPath = "assets/shaders/terrain/vertex.glsl";
	fPath = "assets/shaders/terrain/fragment.glsl";
	path_grass = "assets/terrain/textures/grass1.png";
	path_road = "assets/terrain/textures/road1.png";
	path_rock = "assets/terrain/textures/rock1.png";
	plane_path = "assets/terrain/plane66k.obj";
}


void Terrain::create() {
	//loadingText.create("tahoma_8");


	/* OBJ LOAD */
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(plane_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		printf("Model (%s) failed to load: %s", plane_path, importer.GetErrorString());
		return;
	}

	//-----------------
	load_node(scene->mRootNode, scene);
	//-----------------

	//delete scene;

	create_mesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	texture();
}

void Terrain::load_node(aiNode * node, const aiScene * scene) {
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		load_mesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		load_node(node->mChildren[i], scene);
	}
}


void Terrain::load_mesh(aiMesh * mesh, const aiScene * scene) {

	float *xCoord = new float();
	float *yCoord = new float();
	float *zCoord = new float();
	noiseData *nData = new noiseData;
	aiFace *face = new aiFace;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {  

		if ((int)i % 3000 == 0) {
			glfwSwapBuffers(GLB::MAIN_WINDOW);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//---
			loadingText.set_position(glm::vec2(GLB::WINDOW_WIDTH / 2.f, GLB::WINDOW_HEIGHT / 2.f));
			loadingText.set_text("Map generation in progress...");
			loadingText.render("tahoma_8", glm::vec4(255.f), "center", "middle");
			//---
			loadingText.set_position(glm::vec2(GLB::WINDOW_WIDTH / 2.f, GLB::WINDOW_HEIGHT / 2.f - 50.f));
			loadingText.set_text(std::to_string(int((float)i/(float)mesh->mNumVertices*100.f)) + "%");
			loadingText.render("tahoma_8", glm::vec4(255.f), "center", "middle");
			//---
			glfwSwapBuffers(GLB::MAIN_WINDOW);
		}

		/*-----------------------*/
		/* RANDOM MAP GENERATION */   // Look MapGeneration.h for the functions
		/*-----------------------*/

		/*
			The plane stays in X[0-10000],Y[0-10000],Z[0-0]
			So, we just have to push back the Z in order to have a 45° inclination
		*/

		*xCoord = mesh->mVertices[i].x;
		*yCoord = mesh->mVertices[i].y;
		*zCoord = 0.f;

		/* 45° Inclination */
		(*zCoord) -= (*yCoord);
		glm::vec2 xyCoords = glm::vec2(*xCoord, *yCoord);

		/* Noise calculation */
		*nData = mapgen::generateNoise(glm::vec2(*xCoord, *yCoord));

		(*zCoord) += nData->zNoise;
		(*yCoord) += mapgen::smoothNoise(*yCoord, nData->zNoise);

		MAP::MIN_Z = std::min(nData->zNoise, MAP::MIN_Z);
		MAP::MAX_Z = std::max(nData->zNoise, MAP::MAX_Z);

		vertices.insert(vertices.end(), { *xCoord, *yCoord, *zCoord, nData->zNoise });

		/*-----------------------*/
		// Texture Coords //
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else { vertices.insert(vertices.end(), { 0.0f, 0.0f }); }
		/*-----------------------*/

		/* NORMALS UPDATE */
		glm::vec3 oldNormals = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		
		glm::vec3 N = mapgen::updatedNormals(xyCoords);
		vertices.insert(vertices.end(), { N.x , N.y , N.z });

		/* Terrain data */
		vertices.insert(vertices.end(), { nData->grassWeight,  nData->roadWeight });

		
	}

	glfwSwapBuffers(GLB::MAIN_WINDOW);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//---
	loadingText.set_position(glm::vec2(GLB::WINDOW_WIDTH / 2.f, GLB::WINDOW_HEIGHT / 2.f));
	loadingText.set_text("Finalizing...");
	loadingText.render("tahoma_8", glm::vec4(255.f), "center", "middle");
	//---
	glfwSwapBuffers(GLB::MAIN_WINDOW);

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		*face = mesh->mFaces[i];
		for (size_t j = 0; j < face->mNumIndices; j++) { indices.push_back(face->mIndices[j]); }
	}

	delete xCoord;
	delete yCoord;
	delete zCoord;
	delete nData;
	delete face;
}

void Terrain::create_mesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 4));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 6));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, (void*)(sizeof(vertices[0]) * 9));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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



void Terrain::render(glm::mat4 projMat, glm::mat4 viewMat, bool tracing) {
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
	
	/* Common Uniform Variables */

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(projMat));
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
	
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
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
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
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
		
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


Terrain::~Terrain()
{
}
