#include "filled_rectangle.h"

FilledRectangle::FilledRectangle(){
	vPath = "assets/shaders/filled_rectangle/vertex.glsl";
	fPath = "assets/shaders/filled_rectangle/fragment.glsl";
	color = glm::vec4(0.0f);
}

FilledRectangle::FilledRectangle(int shaderID){
	shaderId = shaderID;
}

void FilledRectangle::set_color(glm::vec4 col){
	color = col;
}

void FilledRectangle::create(float width, float height, std::string origin) {
	w = width;
	h = height;

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
		// positions				
			x0,		y0,		0.0f,		
			x1,		y1,		0.0f,		
			x2,		y2,		0.0f,		
			x3,		y3,		0.0f,		
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void FilledRectangle::render(float xPos, float yPos, bool picking, int picking_id) {

	glUseProgram(shaderId);

	modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
	
	glUniform1i(glGetUniformLocation(shaderId, "picking"), int(picking));  // enable/disable picking

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	if (picking) {
		r = (picking_id & 0x000000FF) >> 0;
		g = (picking_id & 0x0000FF00) >> 8;
		b = (picking_id & 0x00FF0000) >> 16;
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	}

	else {
		glUniform4f(glGetUniformLocation(shaderId, "color"), color.x / 255.0f, color.y / 255.0f, color.z / 255.0f, color.w);
	}

	/* Draw */
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

FilledRectangle::~FilledRectangle()
{
}
