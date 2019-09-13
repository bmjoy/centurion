#define STB_IMAGE_IMPLEMENTATION  
#include <stb_image.h>  // manip. texture

#include "image.h"

Image::Image(){
	vPath = "assets/shaders/image/vertex.glsl";
	fPath = "assets/shaders/image/fragment.glsl";
}

Image::Image(int shaderID){
	vPath = "assets/shaders/image/vertex.glsl";
	fPath = "assets/shaders/image/fragment.glsl";
	shaderId = shaderID;
}

void Image::apply_projection_matrix(glm::mat4 mat) {
	glUseProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, glm::value_ptr(mat));
}

void Image::create(std::string image_path, std::string origin, glm::ivec2 image_size) {

	// load image
	textureInfoList.push_back(glm::ivec3(0, 0, 0)); 
	unsigned char *data = stbi_load(image_path.c_str(), &textureInfoList[0].x, &textureInfoList[0].y, &textureInfoList[0].z, 0);
	if (!data) { std::cout << "Failed to load texture" << std::endl; }

	if (image_size == glm::ivec2(0,0)) {
		w = (float)textureInfoList[0].x;
		h = (float)textureInfoList[0].y;
	}
	else {
		w = (float)image_size.x;
		h = (float)image_size.y;
	}

	if (origin == "center"){//                 |
		x0 = -w / 2.f;		//        x3,y3---------x2,y2
		y0 = -h / 2.f;		//          |      |      |
		x1 = +w / 2.f;		//          |      |      |
		y1 = -h / 2.f;		//----------|-------------|--------
		x2 = +w / 2.f;		//          |      |      |
		y2 = +h / 2.f;		//          |      |      |
		x3 = -w / 2.f;		//        x0,y0---------x1,y1
		y3 = +h / 2.f;		//                 |
	}

	else if (origin == "top-left") {//          |
		x0 = -0;					//--------x3,y3---------x2,y2------
		y0 = -h;					//          |             |
		x1 = +w;					//          |             |
		y1 = -h;					//          |             |
		x2 = +w;					//          |             |
		y2 = +0;					//          |             |
		x3 = -0;					//        x0,y0---------x1,y1
		y3 = +0;					//          |  
									//          |
									//          |
	}

	else if (origin == "bottom-left") {//          |
									   //          |
									   //          |
		x0 = -0;					   //        x3,y3---------x2,y2
		y0 = -0;					   //          |             |
		x1 = +w;					   //          |             |
		y1 = -0;					   //          |             |
		x2 = +w;					   //          |             |
		y2 = +h;					   //          |             |
		x3 = -0;					   //--------x0,y0---------x1,y1---------
		y3 = +h;					   //          |  
	}

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	
	float vertices[] = {
		// positions				// uv coords		
			x0,		y0,		0.0f,		0.0f, 1.0,   // in basso a sx
			x1,		y1,		0.0f,		1.0, 1.0,    // in basso a dx
			x2,		y2,		0.0f,		1.0, 0.0f,   // in alto a dx
			x3,		y3,		0.0f,		0.0f, 0.0f   // in alto a sx
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

	// TEXTURE
	textureIdList.push_back(0);
	glGenTextures(1, &textureIdList[0]);

	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// create texture and generate mipmaps
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[0].x, textureInfoList[0].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

void Image::render(float xPos, float yPos, bool picking, int picking_id) {

	glUseProgram(shaderId);
	modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos , 0.0f));

	/* Uniform Variables */
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform1i(glGetUniformLocation(shaderId, "picking"), int(picking));  // enable/disable picking
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0); // texture

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIdList[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	
	if (picking) {
		r = (picking_id & 0x000000FF) >> 0;
		g = (picking_id & 0x0000FF00) >> 8;
		b = (picking_id & 0x00FF0000) >> 16;
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	/* Draw */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Image::~Image()
{
}
