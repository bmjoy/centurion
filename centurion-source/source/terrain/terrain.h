#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map_generation.h>
#include <shader.h>
#include "light.h"
#include "../gui/div_text.h"
#include "../engine/window.h"

class Terrain : public Shader
{
public:
	Terrain();
	void create();
	void set_window(myWindow *wnd) { window = wnd; }
	void render(glm::mat4 projMat, glm::mat4 viewMat, bool tracing);
	~Terrain();

private:
	myWindow *window;
	DivText loadingText;

	const char* path_grass;
	const char* path_road;
	const char* path_rock;
	const char* plane_path;

	float randomX, randomY;
	int randomPos;

	void load_node(aiNode * node, const aiScene * scene);
	void load_mesh(aiMesh *mesh, const aiScene *scene);
	void create_mesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void texture();
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	int indexCount;

	// light 
	Light light = Light(1.0f, 1.0f, 1.0f, 0.0f,   // ambient light
		0.0f, 0.0f, 1.0f, 1.0f);  // diffuse light (important for pseudo shadows)
};

