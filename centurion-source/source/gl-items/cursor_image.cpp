#include "cursor_image.h"

#include <stb_image.h>

CursorImage::CursorImage(){
	vPath = "assets/shaders/mouse/vertex.glsl";
	fPath = "assets/shaders/mouse/fragment.glsl";
	cursorImgWidth = 36.0; cursorImgHeight = 36.0;
	initPath = "assets/ui/mouse/";
	texturePaths = { initPath + "default.png", initPath + "right.png", initPath + "left.png" };
	textureIds[3] = { 0 };
}

void CursorImage::create() {

	for (int i = 0; i < 3; ++i) {

		unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
		};
		GLfloat vertices[] = {
			// positions						// uv coords		
			0.0,		-cursorImgHeight,			0.0f,		0.0f, 1.0,   // in basso a sx
			cursorImgWidth,			-cursorImgHeight,			0.0f,		1.0, 1.0,    // in basso a dx
			cursorImgWidth,			0.0,		0.0f,		1.0, 0.0f,   // in alto a dx
			0.0,		0.0,		0.0f,		0.0f, 0.0f   // in alto a sx
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

		int width, height, nrChannels;

		unsigned char *data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (!data) { 
			std::cout << "Failed to load texture" << std::endl; 
		}

		// TEXTURE
		glGenTextures(1, &textureIds[i]);
		glBindTexture(GL_TEXTURE_2D, textureIds[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
}

void CursorImage::render(float x, float y, int currentState) {

	glUseProgram(shaderId);

	/* Uniform Variables */
	glUniform1f(glGetUniformLocation(shaderId, "x"), x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), y);
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);

	/* Draw */
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIds[currentState]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

CursorImage::~CursorImage()
{
}
