/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef UNIT_SPRITE_H
#define UNIT_SPRITE_H

#include "object_sprite.h"

class UnitSprite : public ObjectSprite
{
public:

	struct UnitData {
		map<int, int[2]> spriteSize;
		map<int, int> Frames;
		map<int, int> Durations;
		map<string, int> States;
		vec3 playerColor;
		vec3 pickingColor;
		string className;
		int hitBox[2];
		GLint currentState;
		int currentFrame;
		int currentDir;
		int yOffset;
		int maxDirections;
		GLuint textureID;
	};

	UnitSprite();
	void create();
	void getTextureInfo(UnitData *uData);
	void render(UnitData d, vec3 &pos, bool picking);
	~UnitSprite();

private:
	map<string, int[2]> spriteSize;

};

static UnitSprite usprite;

#endif