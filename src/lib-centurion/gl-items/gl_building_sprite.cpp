#include "gl_building_sprite.h"

#include <stb_image.h>

#include <game/strategy.h>
#include <classes/classes-data.h>

#include <file_manager.h>
#include <logger.h>

using namespace std;
using namespace glm;

glBuildingSprite::glBuildingSprite(){
	vPath = "assets/shaders/bsprite/vertex.glsl";
	fPath = "assets/shaders/bsprite/fragment.glsl";
}

void glBuildingSprite::Render(SpriteData &data, float x, float y, bool picking, bool selected, vec3 playerColor, bool placeable) {
	glUseProgram(shaderId);

	/* Uniform Variables */
	glUniform3f(glGetUniformLocation(shaderId, "player_color"), playerColor.x / 255.0f, playerColor.y / 255.0f, playerColor.z / 255.0f);
	glUniform1i(glGetUniformLocation(shaderId, "selection"), selected);
	glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 0);
	glUniform1i(glGetUniformLocation(shaderId, "picking"), picking); // enable/disable picking
	glUniform1i(glGetUniformLocation(shaderId, "not_placeable"), !placeable); 

	glUniform1f(glGetUniformLocation(shaderId, "x"), x);
	glUniform1f(glGetUniformLocation(shaderId, "y"), y);
	glUniform1f(glGetUniformLocation(shaderId, "z"), 10.f);
	glUniform1f(glGetUniformLocation(shaderId, "w"), data.width);
	glUniform1f(glGetUniformLocation(shaderId, "h"), data.height);

	/* ------ DRAW SPRITES ------ */

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glEnable(GL_DEPTH_TEST);

	/* PICKING = TRUE */

	if (picking){
		glUniform4f(glGetUniformLocation(shaderId, "picking_color"), data.pickingColor.r / 255.0f, data.pickingColor.g / 255.0f, data.pickingColor.b / 255.0f, 1.0f);
		if (Game::Minimap::IsActive() == false) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		if (Game::Minimap::IsActive() && data.isClickable) {
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId + 2); // border
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	/* PICKING = FALSE (NORMAL RENDER)*/

	else {
		if (Game::Minimap::IsActive() == false){
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 0);
			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		else {
			glUniform1i(glGetUniformLocation(shaderId, "minimap"), 1);
			
			if (data.isClickable) {
				glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 2);
				glActiveTexture(GL_TEXTURE2); 
				glBindTexture(GL_TEXTURE_2D, data.textureId + 2); // border
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isBorder"), 0);

				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 1);
				glUniform1i(glGetUniformLocation(shaderId, "texture1"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, data.textureId + 1); // color
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glUniform1i(glGetUniformLocation(shaderId, "isLayerColor"), 0);
			}

			glUniform1i(glGetUniformLocation(shaderId, "texture1"), 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, data.textureId); // normal
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glDisable(GL_DEPTH_TEST);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

glBuildingSprite::~glBuildingSprite()
{
}
