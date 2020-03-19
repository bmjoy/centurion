#ifndef UNIT_SPRITE_H
#define UNIT_SPRITE_H

#include "object_sprite.h"

class UnitSprite : public ObjectSprite
{
public:
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