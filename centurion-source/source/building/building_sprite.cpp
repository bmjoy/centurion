#include "stb_image.h"  // manip. texture

#include "building_sprite.h"

BSprite::BSprite(){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
	modelMat = glm::mat4(1.f);
}


BSprite::BSprite(int shaderID){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
	shaderId = shaderID;
}

void BSprite::create() {
	
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

			textureIdList.push_back(0);
			textureInfoList.push_back(glm::ivec3(0, 0, 0));
			texturePath = ent_data["path"].get<std::string>() + ent_data["sprites"][i]["name"].get<std::string>();
			data = stbi_load(texturePath.c_str(), &textureInfoList[k].x, &textureInfoList[k].y, &textureInfoList[k].z, 0);
			if (!data) { std::cout << "Failed to load texture" << std::endl; }

			/* texture */

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

void BSprite::render(std::string className, glm::mat4 model, bool picking, int pickingId, bool selected, glm::vec3 *playerColor) {
	glUseProgram(shaderId);

	/* Uniform Variables */
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3f(glGetUniformLocation(shaderId, "player_color"), playerColor->x / 255.0f, playerColor->y / 255.0f, playerColor->z / 255.0f);
	glUniform1i(glGetUniformLocation(shaderId, "selection"), int(selected));
	glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "picking"), int(picking)); // enable/disable picking

	/* ------ DRAW SPRITES ------ */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	/* PICKING = TRUE */

	if (picking){
		r = (pickingId & 0x000000FF) >> 0;
		g = (pickingId & 0x0000FF00) >> 8;
		b = (pickingId & 0x00FF0000) >> 16;
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		if (!GAME::MINIMAP_IS_ACTIVE) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_normal"]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (GAME::MINIMAP_IS_ACTIVE && textureIdMap[className + "_border"]) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_border"]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	/* PICKING = FALSE (NORMAL RENDER)*/

	else {
		if (!GAME::MINIMAP_IS_ACTIVE){
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 0);
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_normal"]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		else {
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 1);
			
			if (textureIdMap[className + "_border"]) {

				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 2);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_border"]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_color"]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
			}

			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureIdMap[className + "_normal"]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void BSprite::genBuffers() {
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

BSprite::~BSprite()
{
}
