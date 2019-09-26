#pragma once

#include "../primitives/empty_rectangle.h"
#include "../primitives/bitmap_font.h"
#include "../primitives/building_sprite.h"
#include "../primitives/filled_rectangle.h"

namespace obj {

	static BitmapFont txt;
	BitmapFont *Text();

	static BSprite bsprite;
	BSprite *BuildingSprite();

	static EmptyRectangle eRect;
	EmptyRectangle *ERectangle();

	static FilledRectangle fRect;
	FilledRectangle *FRectangle();

	void init();
	void compile();
	void create();
}
