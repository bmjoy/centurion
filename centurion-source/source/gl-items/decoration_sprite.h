#ifndef DECORATIONSPRITE_H
#define DECORATIONSPRITE_H

#include "object_sprite.h"
#include "primitives_structs.h"

class DecorationSprite : public ObjectSprite
{
public:
	DecorationSprite();
	GLuint getTextureID(string classname);
	void create();
	void render(DecorationData d, bool not_placeable = false);
	~DecorationSprite();
};

static DecorationSprite dsprite;

#endif