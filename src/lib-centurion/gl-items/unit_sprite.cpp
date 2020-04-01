#include "unit_sprite.h"

#include <stb_image.h>
#include <fstream>

using namespace std;
using namespace glm;

UnitSprite::UnitSprite(){
	vPath = "assets/shaders/usprite/vertex.glsl";
	fPath = "assets/shaders/usprite/fragment.glsl";
}

void UnitSprite::create() {
	
	//glUseProgram(shaderId);
	//GenerateBuffers();

	//int k = 0;
	//int w, h, nrChannels;
	//GLuint texID;

	//std::string className;
	//std::string fullName;
	//std::string state;

	//for (int j = 0; j < entPathList.size(); ++j) {

	//	std::ifstream ent_path(entPathList[j]);
	//	json ent_data = json::parse(ent_path);

	//	for (int i = 0; i < ent_data["spriteList"].size(); i++) {

	//		w = 0; h = 0; nrChannels = 0; texID = 0;

	//		className = ent_data["class_name"].get<std::string>();
	//		state = ent_data["spriteList"][i].get<std::string>();
	//		fullName = className + "_" + state;
	//		string texturePath = ent_data["sprites"][state]["filePath"].get<std::string>();

	//		/* texture info */
	//		data = stbi_load(texturePath.c_str(), &w, &h, &nrChannels, 0);
	//		if (!data) { std::cout << "Failed to load texture" << std::endl; }

	//		/* texture */
	//		glGenTextures(1, &texID);
	//		glBindTexture(GL_TEXTURE_2D, texID);
	//		// create texture and generate mipmaps
	//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//		glGenerateMipmap(GL_TEXTURE_2D);
	//		glBindTexture(GL_TEXTURE_2D, 0);

	//		stbi_image_free(data);

	//		/* store information */
	//		textureIdMap[fullName] = texID;
	//		spriteSize[fullName][0] = w;
	//		spriteSize[fullName][1] = h;

	//		k++;
	//	}
	//}	
}

void UnitSprite::getTextureInfo(UnitData *uData) {

	(*uData).textureID = textureIdMap[uData->className + "_idle"];
	
	for (std::map<std::string, int>::iterator i = uData->States.begin(); i != uData->States.end(); i++) {
		std::string state = i->first;
		(*uData).spriteSize[uData->States[state]][0] = spriteSize[uData->className + "_" + state][0];
		(*uData).spriteSize[uData->States[state]][1] = spriteSize[uData->className + "_" + state][1];
	}
}

void UnitSprite::render(UnitData d, glm::vec3 &pos, bool picking) {

	glUseProgram(shaderId);

	/* Uniform Variables */

	glUniform1f(glGetUniformLocation(shaderId, "x"), pos.x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), pos.y);
	glUniform1i(glGetUniformLocation(shaderId, "w"), d.spriteSize[d.currentState][0]);
	glUniform1i(glGetUniformLocation(shaderId, "h"), d.spriteSize[d.currentState][1]);
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0); // texture
	glUniform1i(glGetUniformLocation(shaderId, "yOffset"), d.yOffset); // vertical offset
	glUniform1i(glGetUniformLocation(shaderId, "max_directions"), d.maxDirections);
	glUniform1i(glGetUniformLocation(shaderId, "current_frame"), d.currentFrame);
	glUniform1i(glGetUniformLocation(shaderId, "max_frames"), d.Frames[d.currentState]);
	glUniform1i(glGetUniformLocation(shaderId, "current_dir"), d.currentDir);
	
	glUniform1i(glGetUniformLocation(shaderId, "picking"), int(picking));
	glUniform4f(glGetUniformLocation(shaderId, "picking_color"), d.pickingColor.x, d.pickingColor.y, d.pickingColor.z, 1.0f);

	/* Draw */

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, d.textureID + d.currentState);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

UnitSprite::~UnitSprite()
{
}