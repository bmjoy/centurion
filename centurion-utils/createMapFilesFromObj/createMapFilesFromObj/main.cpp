#include "main.h"

unsigned int vertices_pos[146797] = { 0 };

std::string fileName = "plane.obj";
int gridSize = 64;
int mapWidth = 29952;

void load_node(aiNode * node, const aiScene * scene);
void load_mesh(aiMesh *mesh, const aiScene *scene);

int main() {

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

	int xCoord, yCoord, zCoord, zNoise;
	float xTexCoord, yTexCoord;
	int xNorm, yNorm, zNorm;
	int type;

	aiFace face;
	
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	int nVertices = 0;
	std::ofstream myfile("output-folder/vertices");
	std::cout << "Vertices analysis in progress...\n";
	for (size_t i = 0; i < mesh->mNumVertices; i++) {

		xCoord = round(mesh->mVertices[i].x);
		yCoord = round(mesh->mVertices[i].y);
		zCoord = 0;
		zNoise = 0;
		xTexCoord = mesh->mTextureCoords[0][i].x;
		yTexCoord = mesh->mTextureCoords[0][i].y;
		xNorm = 0;
		yNorm = 0;
		zNorm = 1;
		type = 1;

		/* 45° Inclination */
		zCoord -= yCoord;
		
		if (myfile.is_open())
		{
			if (i == 0) {
				myfile << xCoord << "," << yCoord << "," << zCoord << "," << zNoise << "," << xTexCoord << "," << yTexCoord << "," << xNorm << "," << yNorm << "," << zNorm << "," << type;
			}
			else {
				myfile << "," << xCoord << "," << yCoord << "," << zCoord << "," << zNoise << "," << xTexCoord << "," << yTexCoord << "," << xNorm << "," << yNorm << "," << zNorm << "," << type;
			}
		}

		// save vertices position
		int j = yCoord / gridSize * mapWidth / gridSize + xCoord / gridSize;
		vertices_pos[j] = i;

		float perc = 100.f * i / mesh->mNumVertices;
		if (i % 5000 == 0) {
			std::cout << perc << "% \n";
		}

		nVertices++;
	}
	myfile.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	int nIndices = 0;
	std::cout << "\nIndices analysis in progress...\n";
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
			float perc = 100.f * i / mesh->mNumVertices;
			if (i % 5000 == 0) {
				std::cout << perc << "% \n";
			}

			nIndices++;
			k++;
		}
	}
	myfile2.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	std::cout << "\Saving vertices informations...\n";
	std::ofstream myfile3("output-folder/vertices_pos");
	for (int i = 0; i < sizeof(vertices_pos) / sizeof(int); i++) {
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
			std::cout << perc << "% \n";
		}
	}
	myfile3.close();

	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------*/

	std::ofstream myfile4("output-folder/info");
	if (myfile4.is_open())
	{
		myfile4 << "Number of vertices: " << nVertices << "\n";
		myfile4 << "Number of indices: " << nIndices << "\n";
	}
	myfile4.close();

	std::cout << "Number of vertices: " << nVertices << ";\nNumber of indices: " << nIndices << "\n";
}