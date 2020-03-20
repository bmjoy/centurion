#include "building_sprite.h"
#include "entity-xml.hxx"

#include <stb_image.h>
#include <json.hpp>

#include <game/strategy.h>
#include <classes/object-data.h>

#include <file_manager.h>
#include <logger.h>

BuildingSprite::BuildingSprite(){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
}

void BuildingSprite::create() {
	string spritePath = "";
	try
	{
		glUseProgram(shaderId);
		GenerateBuffers();

		std::string fullName;
		SpriteData spriteData = SpriteData();

		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "entity.xsd");
		
		for (int j = 0; j < entPathList.size(); ++j) {
			
			spritePath = entPathList[j];
			fullName = Folders::GAME + entPathList[j];
			string entityFolder = FileManager::GetFileFolderPath(fullName.c_str());

			try {
				auto_ptr<c_entity> entXml = c_entity_(fullName, 0, props);
				std::string _className = string(entXml->class_name());

				ent_images::ent_image_iterator _img_it;
				for (_img_it = entXml->ent_images().ent_image().begin(); _img_it != entXml->ent_images().ent_image().end(); _img_it++) {

					string image_name = string(_img_it->file() + ".png");
					string image_path = entityFolder + "\\" + image_name;

					/* save texture info */
					int width, height, nrChannels;
					GLuint textureID;
					data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
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
					ObjectData::GetObjectData(_className)->SetSpriteData(spriteData);

					stbi_image_free(data);
				}
			}
			catch (const xml_schema::exception & e) {
				string emsg = string(e.what());
				Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the following sprite: \"" + spritePath + "\". This was the error message: \"" + emsg + "\"", "Warn", "", "BuildingSprite", "Create");
				Logger::Warn(msg);
			}
		}
	}
	catch (const std::exception&)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred reading the following sprite: \"" + spritePath + "\"", "Warn", "", "BuildingSprite", "Create");
		Logger::Warn(msg);
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
