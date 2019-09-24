#pragma once

#include "../primitives/bitmap_font.h"
#include "../building/building_sprite.h"

namespace obj {

	static CBitmapFont txt;
	CBitmapFont *Text();

	static BSprite bsprite;
	BSprite *BuildingSprite();

	void init();
	void compile();
	void create();
}
