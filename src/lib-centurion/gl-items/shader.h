/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	/// <summary>
	/// Constructor
	/// </summary>
	Shader();

	/// <summary>
	/// This method compiles the vertex and fragment shaders
	/// </summary>
	void compile();

	/// <summary>
	/// This method applies the projection matrix to the object
	/// </summary>
	void apply_projection_matrix(glm::mat4 mat = glm::mat4(1.0f));
	
	/// <summary>
	/// This method applies the view matrix to the object
	/// </summary>
	void apply_view_matrix(glm::mat4 mat = glm::mat4(1.0f));

	/// <summary>
	/// This method applies the model matrix to the object
	/// </summary>
	void apply_model_matrix(glm::mat4 mat = glm::mat4(1.0f));

	/// <summary>
	/// Destructor
	/// </summary>
	~Shader();

protected:
	const char* vPath;
	const char* fPath;
	int shaderId;
	GLuint VAO, VBO, IBO;
	std::vector<GLuint> textureIdList;
	std::map<std::string, GLuint> textureIdMap;
	std::vector<glm::ivec3> textureInfoList; // width, height, nrChannels

private:
	int success;
	char infoLog[512];
	int vertexShader, fragmentShader;
	const GLchar* vShaderCode;
	const GLchar* fShaderCode;

};

#endif // !SHADER_H