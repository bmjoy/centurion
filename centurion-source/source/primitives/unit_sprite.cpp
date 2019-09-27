#include "stb_image.h"  // manip. texture

#include "unit_sprite.h"

UnitSprite::UnitSprite(){
	vPath = "assets/shaders/usprite/vertex.glsl";
	fPath = "assets/shaders/usprite/fragment.glsl";
}

UnitSprite::UnitSprite(int shaderID){
	vPath = "assets/shaders/usprite/vertex.glsl";
	fPath = "assets/shaders/usprite/fragment.glsl";
	shaderId = shaderID;
}

void UnitSprite::create() {
	
	glUseProgram(shaderId);
	genBuffers();

	int k = 0;

	std::string className;
	std::string fullName;
	std::string state;

	for (int j = 0; j < entPathList.size(); ++j) {

		std::ifstream ent_path(entPathList[j]);
		json ent_data = json::parse(ent_path);

		for (int i = 0; i < ent_data["spriteList"].size(); i++) {

			className = ent_data["class_name"].get<std::string>();
			state = ent_data["spriteList"][i].get<std::string>();
			fullName = className + "_" + state;
			texturePath = ent_data["sprites"][state]["filePath"].get<std::string>();

			/* save texture info */
			textureIdList.push_back(0);
			textureInfoList.push_back(glm::ivec3(0, 0, 0));
			data = stbi_load(texturePath.c_str(), &textureInfoList[k].x, &textureInfoList[k].y, &textureInfoList[k].z, 0);
			if (!data) { std::cout << "Failed to load texture" << std::endl; }

			/* texture */
			glGenTextures(1, &textureIdList[k]);
			glBindTexture(GL_TEXTURE_2D, textureIdList[k]);
			// create texture and generate mipmaps
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[k].x, textureInfoList[k].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(data);
			textureIdMap[fullName] = textureIdList[i];

			spriteSize[fullName][0] = textureInfoList[k].x;
			spriteSize[fullName][1] = textureInfoList[k].y;

			k++;
		}
	}	
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

	if (!GAME::MINIMAP_IS_ACTIVE){
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, d.textureID + d.currentState);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void UnitSprite::genBuffers() {
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

UnitSprite::~UnitSprite()
{
}