#include "primitives.h"

#include "audio_manager.h"
#include "bitmap_font.h"
#include "building_sprite.h"
#include "decoration_sprite.h"
#include "cursor_image.h"
#include "empty_circle.h"
#include "filled_rectangle.h"
#include "empty_rectangle.h"
#include "image_sprite.h"
#include "unit_sprite.h"
#include "terrain.h"
#include "grid.h"
#include "minimap_rectangle.h"

#include <maths.h>
#include <global>

AudioManager *Audio() { return &audioM; }
BitmapFont *Text() { return &_txt; }
BuildingSprite *BSprite() { return &bsprite; }
DecorationSprite *DSprite() { return &dsprite; }
CursorImage *Cursor() { return &cursor; }
EmptyCircle *ECircle() { return &eCircle; }
EmptyRectangle *ERectangle() { return &eRect; }
FilledRectangle *FRectangle() { return &fRect; }
ImageSprite *Img() { return &img; }
UnitSprite *USprite() { return &usprite; }
Terrain *MapTerrain() { return &terrain; }
Grid *MapGrid() { return &grid; }
MinimapRectangle *MMRectangle() { return &mmRect; }

void init() {
	//*Audio() = AudioManager();
	*Text() = BitmapFont();
	*BSprite() = BuildingSprite();
	*DSprite() = DecorationSprite();
	*Cursor() = CursorImage();
	*ECircle() = EmptyCircle();
	*ERectangle() = EmptyRectangle();
	*FRectangle() = FilledRectangle();
	*Img() = ImageSprite();
	*USprite() = UnitSprite();
	*MapTerrain() = Terrain();
	*MapGrid() = Grid();
	*MMRectangle() = MinimapRectangle();
}
void compile() {
	Text()->compile();
	BSprite()->compile();
	DSprite()->compile();
	ECircle()->compile();
	ERectangle()->compile();
	FRectangle()->compile();
	Cursor()->compile();
	Img()->compile();
	USprite()->compile();
	MapTerrain()->compile();
	MapGrid()->compile();
	MMRectangle()->compile();
}
void create() {
	Text()->apply_projection_matrix(glb::menuProjection);
	Cursor()->apply_projection_matrix(glb::menuProjection);

	//----

	Text()->create();
	Cursor()->create();
	ECircle()->create();
	ERectangle()->create();
	FRectangle()->create();
	BSprite()->create();
	DSprite()->create();
	Img()->create();
	USprite()->create();

	//----

	MapTerrain()->create();
	MapGrid()->create();
	MMRectangle()->create();
}

void applyMenuMatrices() {
	Img()->apply_projection_matrix(glb::menuProjection);
	Img()->apply_view_matrix();
	ECircle()->apply_projection_matrix(glb::menuProjection);
	ECircle()->apply_view_matrix();
	ERectangle()->apply_projection_matrix(glb::menuProjection);
	ERectangle()->apply_view_matrix();
	FRectangle()->apply_projection_matrix(glb::menuProjection);
	FRectangle()->apply_view_matrix();
	MMRectangle()->apply_projection_matrix(glb::menuProjection);
	MMRectangle()->apply_view_matrix();
}
void applyGameMatrices(mat4 *proj, mat4 *view) {
	BSprite()->apply_projection_matrix(*proj);
	BSprite()->apply_view_matrix(*view);
	DSprite()->apply_projection_matrix(*proj);
	DSprite()->apply_view_matrix(*view);
	USprite()->apply_projection_matrix(*proj);
	USprite()->apply_view_matrix(*view);
	Img()->apply_projection_matrix(*proj);
	Img()->apply_view_matrix(*view);
	ECircle()->apply_projection_matrix(*proj);
	ECircle()->apply_view_matrix(*view);
	ERectangle()->apply_projection_matrix(*proj);
	ERectangle()->apply_view_matrix(*view);
	FRectangle()->apply_projection_matrix(*proj);
	FRectangle()->apply_view_matrix(*view);
	MapTerrain()->apply_projection_matrix(*proj);
	MapTerrain()->apply_view_matrix(*view);
	MapGrid()->apply_projection_matrix(*proj);
	MapGrid()->apply_view_matrix(*view);
}