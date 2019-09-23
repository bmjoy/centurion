#include "stb_image.h"  // manip. texture

#include "mouse.h"

Mouse::Mouse()
{
	vPath = "assets/shaders/mouse/vertex.glsl";
	fPath = "assets/shaders/mouse/fragment.glsl";
	
	w = 36.0; h = 36.0;
	initPath = "assets/ui/mouse/";
	texturePaths = { initPath + "default.png", initPath + "right.png", initPath+"left.png"};
	textureNames = { "default", "right", "left" };
	currentState = "default";
}


void Mouse::create() {

	for (int i = 0; i < 3; ++i) {
		unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
		};
		GLfloat vertices[] = {
			// positions						// uv coords		
			0.0,		-h,			0.0f,		0.0f, 1.0,   // in basso a sx
			w,			-h,			0.0f,		1.0, 1.0,    // in basso a dx
			w,			0.0,		0.0f,		1.0, 0.0f,   // in alto a dx
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

		textureIdMap[textureNames[i]] = 0;
		textureInfoList.push_back(glm::ivec3(0, 0, 0));

		unsigned char *data = stbi_load(texturePaths[i].c_str(), &textureInfoList[0].x, &textureInfoList[0].y, &textureInfoList[0].z, 0);
		if (!data) { std::cout << "Failed to load texture" << std::endl; }

		// TEXTURE
		glGenTextures(1, &textureIdMap[textureNames[i]]);
		glBindTexture(GL_TEXTURE_2D, textureIdMap[textureNames[i]]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureInfoList[0].x, textureInfoList[0].y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}
}


void Mouse::render() {

	glUseProgram(shaderId);

	modelMat = glm::translate(glm::mat4(1.0f), position);

	/* Uniform Variables */

	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);

	/* Draw */

	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureIdMap[currentState]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mouse::mouse_control(int lastX, int lastY) {

	position.x = (GLfloat)lastX;
	position.y = (GLfloat)lastY;

	GLB::MOUSE_X = (int)position.x;
	GLB::MOUSE_Y = (int)position.y;
	
	yzoomed = getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y;
	znoise = mapgen::smoothNoise(yzoomed, GLB::Z_NOISE);
	znoise /= (float)GLB::WINDOW_HEIGHT_ZOOMED / (float)GLB::WINDOW_HEIGHT;

	GLB::MOUSE_Y_2D = (GLB::MOUSE_Y - (int)znoise);
	


	if (!GLB::MOUSE_LEFT) {
		GLB::MOUSE_LEFT_X = (int)lastX;
		GLB::MOUSE_LEFT_Y = (int)lastY;
		
	}
	else {
		currentState = "left";
		printf("You have left-clicked on (X=%d, Y=%d)\n", GLB::MOUSE_LEFT_X, GLB::MOUSE_LEFT_Y);
	}

	if (!GLB::MOUSE_RIGHT) {
		GLB::MOUSE_RIGHT_X = GLB::MOUSE_X;
		GLB::MOUSE_RIGHT_Y = GLB::MOUSE_Y;
		GLB::MOUSE_RIGHT_Y_2D = GLB::MOUSE_Y_2D;
	}
	else {
		currentState = "right";
		printf("You have right-clicked on (X=%d, Y=%d)\n", GLB::MOUSE_RIGHT_X, GLB::MOUSE_RIGHT_Y);
	}

	if (GLB::MOUSE_RELEASE) {
		currentState = "default";
	}
}

Mouse::~Mouse()
{
}
