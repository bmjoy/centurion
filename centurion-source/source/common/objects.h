#pragma once

#include "../primitives/empty_rectangle.h"
#include "../primitives/bitmap_font.h"
#include "../primitives/building_sprite.h"
#include "../primitives/filled_rectangle.h"
#include "../primitives/cursor_image.h"
#include "../primitives/unit_sprite.h"

namespace obj {

	static BitmapFont txt;
	BitmapFont *Text();

	static BuildingSprite bsprite;
	BuildingSprite *BSprite();

	static CursorImage cursor;
	CursorImage *Cursor();

	static EmptyRectangle eRect;
	EmptyRectangle *ERectangle();

	static FilledRectangle fRect;
	FilledRectangle *FRectangle();

	static UnitSprite usprite;
	UnitSprite *USprite();

	void init();
	void compile();
	void create();
}
