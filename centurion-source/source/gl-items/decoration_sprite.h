#ifndef DECORATIONSPRITE_H
#define DECORATIONSPRITE_H

#include "object_sprite.h"
#include "primitives_structs.h"

class DecorationSprite : public ObjectSprite
{
public:
	DecorationSprite();
	void Render(SpriteData &data, float x, float y, bool not_placeable = false);
	~DecorationSprite();
};

static DecorationSprite dsprite;

#endif