#include "building_sprite.h"
#include <stb_image.h>
#include <json.hpp>

#include <game/strategy.h>
#include <classes/object-data.h>


BuildingSprite::BuildingSprite(){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
}

void BuildingSprite::create() {
	
	glUseProgram(shaderId);
	GenerateBuffers();
	
	//int k = 0;

	std::string className;
	std::string fullName;
	SpriteData spriteData = SpriteData();

	for (int j = 0; j < entPathList.size(); ++j) {

		std::ifstream ent_path(entPathList[j]);
		json ent_data = json::parse(ent_path);
		string texturePath;

		for (int i = 0; i < ent_data["sprites"].size(); i++) {

			className = ent_data["class_name"].get<std::string>();
			fullName = className + "_" + ent_data["sprites"][i]["type"].get<std::string>();
			texturePath = ent_data["path"].get<std::string>() + ent_data["sprites"][i]["name"].get<std::string>();

			/* save texture info */
			int width, height, nrChannels;
			GLuint textureID;
			data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
			if (!data) { 
				std::cout << "Failed to load texture" << std::endl; 
			}

			/* load texture to gpu */
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// create texture and generate mipmaps
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			// save sprite data
			spriteData.width = float(width);
			spriteData.height = float(height);
			spriteData.textureId = textureID;

			// send data to objectData via classname
			ObjectData::GetObjectData(className)->SetSpriteData(spriteData);

			stbi_image_free(data);			
		}
	}	
}

void BuildingSprite::render(SpriteData &data, float x, float y, bool picking, bool selected, vec3 playerColor, bool not_placeable) {
	glUseProgram(shaderId);

	/* Uniform Variables */
	glUniform3f(glGetUniformLocation(shaderId, "player_color"), playerColor.x / 255.0f, playerColor.y / 255.0f, playerColor.z / 255.0f);
	glUniform1i(glGetUniformLocation(shaderId, "selection"), selected);
	glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "picking"), picking); // enable/disable picking
	glUniform1i(glGetUniformLocation(shaderId, "not_placeable"), not_placeable); // enable/disable picking

	glUniform1f(glGetUniformLocation(shaderId, "x"), x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), y);
	glUniform1f(glGetUniformLocation(shaderId, "z"), 10.f);
	glUniform1f(glGetUniformLocation(shaderId, "w"), data.width);
	glUniform1f(glGetUniformLocation(shaderId, "h"), data.height);

	/* ------ DRAW SPRITES ------ */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glEnable(GL_DEPTH_TEST);

	/* PICKING = TRUE */

	if (picking){
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), data.pickingColor.r / 255.0f, data.pickingColor.g / 255.0f, data.pickingColor.b / 255.0f, 1.0f);
		if (Game::Minimap::IsActive() == false) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (Game::Minimap::IsActive() && data.isClickable) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId + 2); // border
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	/* PICKING = FALSE (NORMAL RENDER)*/

	else {
		if (Game::Minimap::IsActive() == false){
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 0);
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		else {
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 1);
			
			if (data.isClickable) {
				glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 2);
				glActiveTexture(GL_TEXTURE2); 
				glBindTexture(GL_TEXTURE_2D, data.textureId + 2); // border
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 0);

				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, data.textureId + 1); // color
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
			}

			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

BuildingSprite::~BuildingSprite()
{
}
