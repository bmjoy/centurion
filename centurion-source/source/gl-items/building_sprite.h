#ifndef BUILDINGSPRITE_H
#define BUILDINGSPRITE_H

#include "object_sprite.h"

class BuildingSprite : public ObjectSprite
{
public:
	BuildingSprite();
	void create();
	void Render(SpriteData &data, float x, float y, bool picking, bool selected, vec3 playerColor, bool not_placeable = false);
	~BuildingSprite();
};

static BuildingSprite bsprite;

#endif