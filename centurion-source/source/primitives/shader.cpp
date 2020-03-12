#include "shader.h"

#include <sstream>

#include <file_manager.h>
#include <logger.h>

Shader::Shader() {
	VAO = 0; VBO = 0; IBO = 0;
	shaderId = 0;
};

void Shader::compile() {
	// Compile and setup the shader
	std::string vertexString = FileManager::ReadFile(vPath);
	std::string fragmentString = FileManager::ReadFile(fPath);
	vShaderCode = vertexString.c_str();
	fShaderCode = fragmentString.c_str();

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		stringstream ss;
		
		ss << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog;
		Logger::Error(ss.str());
	}
	// fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		stringstream ss;

		ss << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog;
		Logger::Error(ss.str());
	}
	// link shaders
	shaderId = glCreateProgram();
	glAttachShader(shaderId, vertexShader);
	glAttachShader(shaderId, fragmentShader);
	glLinkProgram(shaderId);
	// check for linking errors
	glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
		stringstream ss;

		ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog;
		Logger::Error(ss.str());
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::apply_projection_matrix(glm::mat4 mat) {
	glUseProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::apply_view_matrix(glm::mat4 mat) {
	glUseProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "view"), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::apply_model_matrix(glm::mat4 mat) {
	glUseProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(mat));
}

Shader::~Shader() {}