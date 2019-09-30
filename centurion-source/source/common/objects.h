#pragma once

#include "../primitives/primitives.h"

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

	static ImageSprite img;
	ImageSprite *Img();

	static UnitSprite usprite;
	UnitSprite *USprite();

	static Terrain terrain;
	Terrain *MapTerrain();

	void applyMenuMatrices();
	void applyGameMatrices(glm::mat4 *proj, glm::mat4 *view);
	void init();
	void compile();
	void create();
}
