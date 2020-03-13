#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <primitives_structs.h>

using namespace glm;

class AudioManager;
class BitmapFont;
class BuildingSprite;
class CursorImage;
class DecorationSprite;
class EmptyRectangle;
class EmptyCircle;
class FilledRectangle;
class ImageSprite;
class UnitSprite;
class Terrain;
class Grid;
class MinimapRectangle;

namespace primitives {
	AudioManager *Audio();
	BitmapFont *Text();
	BuildingSprite *BSprite();
	CursorImage *Cursor();
	DecorationSprite *DSprite();
	EmptyRectangle *ERectangle();
	EmptyCircle *ECircle();
	FilledRectangle *FRectangle();
	ImageSprite *Img();
	UnitSprite *USprite();
	Terrain *MapTerrain();
	Grid *MapGrid();
	MinimapRectangle *MMRectangle();

	static mat4 MENU_PROJECTION_MATRIX = mat4(0);
	static mat4 CAMERA_PROJECTION_MATRIX = mat4(0);
	static mat4 MINIMAP_PROJECTION_MATRIX = mat4(0);

	void setMenuProjectionMatrix(mat4 mat);
	void setCameraProjectionMatrix(mat4 mat);
	void setMinimapProjectionMatrix();
	mat4 getCameraProjectionMatrix(void);
	mat4 getMinimapProjectionMatrix(void);

	void applyMenuMatrices();
	void applyGameMatrices(mat4 *proj, mat4 *view);
	void init();
	void compile();
	void create();
};

#endif