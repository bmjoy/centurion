#include "bitmap_font.h"

#include <stb_image.h>  // manip. texture

CBitmapFont::CBitmapFont() {
	vPath = "assets/shaders/font/vertex.glsl";
	fPath = "assets/shaders/font/fragment.glsl";
	h_align = "left";
	v_align = "normal";
}

CBitmapFont::CBitmapFont(int shader_id) {
	shaderId = shader_id;
	h_align = "left";
	v_align = "normal";
}

void CBitmapFont::set_align(std::string hAlign, std::string vAlign) {
	h_align = hAlign;
	v_align = vAlign;
}

void CBitmapFont::create(std::string font) {

	path = "assets/fonts/" + font + ".png";

	// load image
	textureInfoList.push_back(glm::ivec3(0, 0, 0));
	unsigned char *data = stbi_load(path.c_str(), &textureInfoList[0].x, &textureInfoList[0].y, &textureInfoList[0].z, 0);
	if (!data) { std::cout << "Failed to load texture" << std::endl; }

	path = "assets/fonts/" + font + ".json";

	std::ifstream json_path(path);
	font_data = json::parse(json_path);

	w = font_data["Cell Width"];
	h = font_data["Cell Height"];
	
	nrows = (float)font_data["Image Width"] / (float)font_data["Cell Width"];
	ncols = (float)font_data["Image Height"] / (float)font_data["Cell Height"];

	glUniform1f(glGetUniformLocation(shaderId, "max_x"), nrows);
	glUniform1f(glGetUniformLocation(shaderId, "max_y"), ncols);

	// origin = bottom-left

	unsigned int indices[] = {
		 0, 1, 3,   // first triangle
		 1, 2, 3    // second triangle
	};

	float vertices[] = {
		// positions	// uv coords		
		0.f,	0.f,	0.0f, 1.0,   // in basso a sx
		w,		0.f,	1.0, 1.0,    // in basso a dx
		w,		h,		1.0, 0.0f,   // in alto a dx
		0.f,	h,		0.0f, 0.0f   // in alto a sx
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// TEXTURE
	textureIdList.push_back(0);
	glGenTextures(1, &textureIdList[0]);

	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[0].x, textureInfoList[0].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

}

void CBitmapFont::render(float xPos, float yPos, std::string text, glm::vec4 color) {
	glUseProgram(shaderId);

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glUniform1i(glGetUniformLocation(shaderId, "tex"), 0); // texture
	glUniform4f(glGetUniformLocation(shaderId, "color"), color.x/255.f, color.y / 255.f, color.z / 255.f, color.w / 255.f);

	offset_x = 0.f;

	total_width = 0.f;

	if (h_align != "left"){
		for (int i = 0; i < text.size(); i++) {
			total_width += (float)font_data["Char " + std::to_string(text.c_str()[i]) + " Base Width"];
		}
	}

	for (int i = 0; i < text.size(); i++) {

		if (h_align == "left" && v_align == "normal") {
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos + offset_x, yPos, 0.0f));
		}
		else if (h_align == "center" && v_align == "normal"){
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos + offset_x - total_width / 2.f, yPos, 0.0f));
		}
		else if (h_align == "left" && v_align == "middle") {
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos + offset_x, yPos - (float)font_data["Font Height"]/2.f, 0.0f));
		}
		else if (h_align == "center" && v_align == "middle") {
			modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos + offset_x - total_width / 2.f, yPos - (float)font_data["Font Height"] / 2.f, 0.0f));
		}

		/* Uniform Variables */
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		

		glUniform1f(glGetUniformLocation(shaderId, "current_x"), (float)((text.c_str()[i] - (int)font_data["Start Char"]) % (int)nrows));
		glUniform1f(glGetUniformLocation(shaderId, "current_y"), (float)((text.c_str()[i] - (int)font_data["Start Char"]) / (int)ncols));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		offset_x += (float)font_data["Char " + std::to_string(text.c_str()[i]) + " Base Width"];
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


CBitmapFont::~CBitmapFont() {
}