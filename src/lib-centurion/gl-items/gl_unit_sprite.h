/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GL_UNIT_SPRITE_H
#define GL_UNIT_SPRITE_H

#include "gl_object_sprite.h"

class glUnitSprite : public glObjectSprite
{
public:

	struct UnitData {
		std::map<int, int[2]> spriteSize;
		std::map<int, int> Frames;
		std::map<int, int> Durations;
		std::map<std::string, int> States;
		glm::vec3 playerColor;
		glm::vec3 pickingColor;
		std::string className;
		int hitBox[2];
		GLint currentState;
		int currentFrame;
		int currentDir;
		int yOffset;
		int maxDirections;
		GLuint textureID;
	};

	glUnitSprite();
	void create();
	void getTextureInfo(UnitData *uData);
	void render(UnitData d, glm::vec3 &pos, bool picking);
	~glUnitSprite();

private:
	std::map<std::string, int[2]> spriteSize;

};

#endif
