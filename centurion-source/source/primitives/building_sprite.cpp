#include <primitives>
#include <stb_image.h>
#include <json.hpp>
#include <global>
#include <game>

using namespace game;

BuildingSprite::BuildingSprite(){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
}


BuildingSprite::BuildingSprite(int shaderID){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
	shaderId = shaderID;
}

void BuildingSprite::create() {
	
	glUseProgram(shaderId);
	genBuffers();
	
	int k = 0;

	std::string className;
	std::string fullName;

	for (int j = 0; j < entPathList.size(); ++j) {

		std::ifstream ent_path(entPathList[j]);
		json ent_data = json::parse(ent_path);

		for (int i = 0; i < ent_data["sprites"].size(); i++) {

			className = ent_data["class_name"].get<std::string>();
			fullName = className + "_" + ent_data["sprites"][i]["type"].get<std::string>();
			texturePath = ent_data["path"].get<std::string>() + ent_data["sprites"][i]["name"].get<std::string>();

			/* save texture info */
			textureIdList.push_back(0);
			textureInfoList.push_back(glm::ivec3(0, 0, 0));	
			data = stbi_load(texturePath.c_str(), &textureInfoList[k].x, &textureInfoList[k].y, &textureInfoList[k].z, 0);
			if (!data) { std::cout << "Failed to load texture" << std::endl; }

			/* load texture to gpu */
			glGenTextures(1, &textureIdList[k]);
			glBindTexture(GL_TEXTURE_2D, textureIdList[k]);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// create texture and generate mipmaps
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[k].x, textureInfoList[k].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);			
			textureIdMap[fullName] = textureIdList[k];

			k++;
		}
	}	
}

GLuint BuildingSprite::getTextureId(std::string className) {
	return textureIdMap[className + "_normal"];
}

void BuildingSprite::render(GLuint texID, bool clickable, float x, float y, float w, float h, bool picking, int pickingId, bool selected, glm::vec3 *playerColor) {
	glUseProgram(shaderId);

	/* Uniform Variables */
	//glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderId, "player_color"), playerColor->x / 255.0f, playerColor->y / 255.0f, playerColor->z / 255.0f);
	glUniform1i(glGetUniformLocation(shaderId, "selection"), selected);
	glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "picking"), picking); // enable/disable picking

	glUniform1f(glGetUniformLocation(shaderId, "x"), x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), y);
	glUniform1f(glGetUniformLocation(shaderId, "w"), w);
	glUniform1f(glGetUniformLocation(shaderId, "h"), h);

	/* ------ DRAW SPRITES ------ */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	/* PICKING = TRUE */

	if (picking){
		r = (pickingId & 0x000000FF) >> 0;
		g = (pickingId & 0x0000FF00) >> 8;
		b = (pickingId & 0x00FF0000) >> 16;
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		if (!gameMinimapStatus) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID); // normal
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (gameMinimapStatus && clickable) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID + 2); // border
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	/* PICKING = FALSE (NORMAL RENDER)*/

	else {
		if (!gameMinimapStatus){
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 0);
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		else {
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 1);
			
			if (clickable) {

				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 2);
				glActiveTexture(GL_TEXTURE2); 
				glBindTexture(GL_TEXTURE_2D, texID + 2); // border
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, texID + 1); // color
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
			}

			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BuildingSprite::genBuffers() {
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

BuildingSprite::~BuildingSprite()
{
}
