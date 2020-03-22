#ifndef SHADER_H
#define SHADER_H

#include "primitives_structs.h"

class Shader {
public:
	Shader();
	void compile();
	void apply_projection_matrix(mat4 mat = mat4(1.0f));
	void apply_view_matrix(mat4 mat = mat4(1.0f));
	void apply_model_matrix(mat4 mat = mat4(1.0f));
	~Shader();

protected:
	const char* vPath;
	const char* fPath;
	int shaderId;
	GLuint VAO, VBO, IBO;
	vector<GLuint> textureIdList;
	map<string, GLuint> textureIdMap;
	vector<ivec3> textureInfoList; // width, height, nrChannels

private:
	int success;
	char infoLog[512];
	int vertexShader, fragmentShader;
	const GLchar* vShaderCode;
	const GLchar* fShaderCode;

};

#endif // !SHADER_H