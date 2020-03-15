#include "decoration_sprite.h"
#include <stb_image.h>
#include <json.hpp>
#include <fstream>


DecorationSprite::DecorationSprite(){
	vPath = "assets/shaders/dsprite/vertex.glsl";
	fPath = "assets/shaders/dsprite/fragment.glsl";
}

void DecorationSprite::create() {
	
	glUseProgram(shaderId);
	genBuffers();

	int k = 0;
	int w, h, nrChannels;
	GLuint texID;

	for (int j = 0; j < entPathList.size(); ++j) {

		std::ifstream ent_path(entPathList[j]);
		json ent_data = json::parse(ent_path);

		string className = ent_data["class_name"].get<string>();
		w = 0; h = 0; nrChannels = 0; texID = 0;
		string texturePath = ent_data["path"].get<string>() + ent_data["sprite"].get<string>();

		/* texture info */
		data = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
		if (!data) { std::cout << "Failed to load texture" << std::endl; }

		/* texture */
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		// create texture and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);

		/* store information */
		textureIdMap[className] = texID;

		k++;
	}	
}

GLuint DecorationSprite::getTextureID(string classname) {
	return textureIdMap[classname];
}

void DecorationSprite::render(DecorationData d, bool not_placeable) {

	glUseProgram(shaderId);

	/* Uniform Variables */

	glUniform1f(glGetUniformLocation(shaderId, "x"), d.position.x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), d.position.y);
	glUniform1f(glGetUniformLocation(shaderId, "z"), 10.f);
	glUniform1i(glGetUniformLocation(shaderId, "w"), d.width);
	glUniform1i(glGetUniformLocation(shaderId, "h"), d.height);
	glUniform1i(glGetUniformLocation(shaderId, "not_placeable"), (int)not_placeable);
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0); // texture

	/* Draw */

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d.textureID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

void DecorationSprite::genBuffers() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

DecorationSprite::~DecorationSprite()
{
}