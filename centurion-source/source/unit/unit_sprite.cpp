#include "stb_image.h"  // manip. texture

#include "unit_sprite.h"

USprite::USprite(){
	vPath = "assets/shaders/usprite/vertex.glsl";
	fPath = "assets/shaders/usprite/fragment.glsl";
}

USprite::USprite(int shaderID){
	vPath = "assets/shaders/usprite/vertex.glsl";
	fPath = "assets/shaders/usprite/fragment.glsl";
	shaderId = shaderID;
}

void USprite::create(json eData) {
	
	entityData = eData;

	glUseProgram(shaderId);

	for (int i = 0; i < entityData["spriteList"].size(); i++) {

		state = entityData["spriteList"][i].get<std::string>();
		textureIdList.push_back(0);
		textureInfoList.push_back(glm::ivec3(0, 0, 0));

		unsigned char *data = stbi_load((entityData["sprites"][state]["filePath"].get<std::string>()).c_str(), &textureInfoList[i].x, &textureInfoList[i].y, &textureInfoList[i].z, 0);
		if (!data) { std::cout << "Failed to load texture" << std::endl; }
		
		glUniform1i(glGetUniformLocation(shaderId, "max_directions"), entityData["sprites"][state]["directions"]);

		w = textureInfoList[i].x / (float)entityData["sprites"][state]["frames"] / 2.0f;
		h = textureInfoList[i].y / (float)entityData["sprites"][state]["directions"] / 2.0f;

		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		GLfloat vertices[] = {
			// positions				// uv coords		
			-w,		-h,		0.0f,		0.0f,		1.0,   // in basso a sx
			w,		-h,		0.0f,		1.0,		1.0,    // in basso a dx
			w,		h,		0.0f,		1.0,		0.0f,   // in alto a dx
			-w,		h,		0.0f,		0.0f,		0.0f   // in alto a sx
		};

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

		/* texture */

		glGenTextures(1, &textureIdList[i]);
		glBindTexture(GL_TEXTURE_2D, textureIdList[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// create texture and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[i].x, textureInfoList[i].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
		textureIdMap[state] = textureIdList[i];
	}	
}

void USprite::render(glm::mat4 modelMat, std::string state, bool picking, int pickingId, glm::vec3 *playerColor) {

	glUseProgram(shaderId);

	/* Uniform Variables */

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform1i(glGetUniformLocation(shaderId, "picking"), int(picking));
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0); // texture
	glUniform1i(glGetUniformLocation(shaderId, "yOffset"), entityData["yOffset"]); // vertical offset

	/* PICKING = TRUE */

	if (picking) {
		r = (pickingId & 0x000000FF) >> 0;
		g = (pickingId & 0x0000FF00) >> 8;
		b = (pickingId & 0x00FF0000) >> 16;
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	}

	/* Draw */

	if (!GAME::MINIMAP_IS_ACTIVE){
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureIdMap[state]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void USprite::set_frame(int f, int mf) {
	glUseProgram(shaderId);
	glUniform1i(glGetUniformLocation(shaderId, "current_frame"), f);
	glUniform1i(glGetUniformLocation(shaderId, "max_frames"), mf);
}

void USprite::set_direction(int d) {
	glUseProgram(shaderId);
	glUniform1i(glGetUniformLocation(shaderId, "current_dir"), d);
}

USprite::~USprite()
{
}