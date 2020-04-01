#include "main.h"

std::string fileName;
int gridSize;
int mapWidth;

void load_node(aiNode * node, const aiScene * scene);
void load_mesh(aiMesh *mesh, const aiScene *scene);



int main() {

	

	std::cout << "File name: ";
	std::cin >> fileName;

	std::cout << "Grid size: ";
	std::cin >> gridSize;

	std::cout << "Map width: ";
	std::cin >> mapWidth;

	/* OBJ LOAD */
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile("input-folder/" + fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		printf("Model (%s) failed to load: %s", "input-folder/" + fileName, importer.GetErrorString());
		return 0;
	}

	load_node(scene->mRootNode, scene);

	while (true);

	return 0;
}

void load_node(aiNode * node, const aiScene * scene) {
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		load_mesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		load_node(node->mChildren[i], scene);
	}
}

void load_mesh(aiMesh * mesh, const aiScene * scene) {

	int xCoord, yCoord, zNoise;
	float xTexCoord, yTexCoord;
	int xNorm, yNorm, zNorm;
	int type;

	aiFace face;
	
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	int nVertices = mesh->mNumVertices;
	std::ofstream verticesFile("output-folder/vertices");
	std::ofstream heightsFile("output-folder/heights");
	std::ofstream texturesFile("output-folder/textures");
	std::cout << "\nVertices analysis in progress...\n\n";

	std::vector<unsigned int> vertices_pos(nVertices, 0);

	for (size_t i = 0; i < mesh->mNumVertices; i++) {

		xCoord = round(mesh->mVertices[i].x);
		yCoord = round(mesh->mVertices[i].y);
		zNoise = 0;
		xTexCoord = mesh->mTextureCoords[0][i].x;
		yTexCoord = mesh->mTextureCoords[0][i].y;
		xNorm = 0;
		yNorm = 0;
		zNorm = 1;
		type = 1;

		/* 45� Inclination */
		//zCoord -= yCoord;
		
		if (verticesFile.is_open())
		{
			if (i == 0) {
				verticesFile << xCoord << "," << yCoord << "," << xTexCoord << "," << yTexCoord;
			}
			else {
				verticesFile << "," << xCoord << "," << yCoord << "," << xTexCoord << "," << yTexCoord;
			}
		}
		if (heightsFile.is_open())
		{
			if (i == 0) {
				heightsFile << zNoise << "," << xNorm << "," << yNorm << "," << zNorm;
			}
			else {
				heightsFile << "," << zNoise << "," << xNorm << "," << yNorm << "," << zNorm;
			}
		}
		if (texturesFile.is_open())
		{
			if (i == 0) {
				texturesFile << type;
			}
			else {
				texturesFile << "," << type;
			}
		}

		// save vertices position
		int j = yCoord / gridSize * mapWidth / gridSize + xCoord / gridSize;
		vertices_pos[j] = i;
		

		float perc = 100.f * i / mesh->mNumVertices;
		if (i % 5000 == 0) {
			std::cout << int(perc) << "%----";
		}
	}
	std::cout << "100%";
	verticesFile.close();
	heightsFile.close();
	texturesFile.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	int nIndices = 0;
	std::cout << "\n\nIndices analysis in progress...\n\n";
	std::ofstream myfile2("output-folder/indices");
	int k = 0;
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) { 

			int index = face.mIndices[j];
			if (myfile2.is_open())
			{
				if (k == 0) {
					myfile2 << index;
				}
				else {
					myfile2 << "," << index;
				}
			}
			float perc = 100.f * k / (face.mNumIndices * mesh->mNumFaces);
			if (k % 10000 == 0) {
				std::cout << int(perc) << "%----";
			}

			nIndices++;
			k++;
		}
	}
	std::cout << "100%";
	myfile2.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	std::cout << "\n\nSaving vertices informations...\n\n";
	std::ofstream myfile3("output-folder/vertices_pos");
	for (int i = 0; i < nVertices; i++) {
		if (myfile3.is_open())
		{
			if (i == 0) {
				myfile3 << vertices_pos[i];
			}
			else {
				myfile3 << "," << vertices_pos[i];
			}
			
			
		}
		float perc = 100.f * i / mesh->mNumVertices;
		if (i % 5000 == 0) {
			std::cout << int(perc) << "%----";
		}
	}
	std::cout << "100%";
	myfile3.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	std::ofstream myfile4("output-folder/info.txt");
	if (myfile4.is_open())
	{
		myfile4 << "Number of vertices: " << nVertices << "\n";
		myfile4 << "Number of indices: " << nIndices << "\n";
		myfile4 << "Grid size: " << gridSize << "\n";
		myfile4 << "Map width: " << mapWidth << "\n";
	}
	myfile4.close();

	std::cout << "\n\nNumber of vertices: " << nVertices << ";\n\nNumber of indices: " << nIndices << "\n";
}