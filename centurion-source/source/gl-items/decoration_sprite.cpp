#include "decoration_sprite.h"
#include <stb_image.h>
#include <fstream>


DecorationSprite::DecorationSprite(){
	vPath = "assets/shaders/dsprite/vertex.glsl";
	fPath = "assets/shaders/dsprite/fragment.glsl";
}

void DecorationSprite::Render(SpriteData &data, float x, float y, bool not_placeable) {

	glUseProgram(shaderId);

	/* Uniform Variables */

	glUniform1f(glGetUniformLocation(shaderId, "x"), x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), y);
	glUniform1f(glGetUniformLocation(shaderId, "z"), 10.f);
	glUniform1f(glGetUniformLocation(shaderId, "w"), data.width);
	glUniform1f(glGetUniformLocation(shaderId, "h"), data.height);
	glUniform1i(glGetUniformLocation(shaderId, "not_placeable"), (int)not_placeable);
	glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0); // texture

	/* Draw */

	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, data.textureId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisable(GL_DEPTH_TEST);
}

DecorationSprite::~DecorationSprite()
{
}