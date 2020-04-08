#include "gl_items.h"

#include "audio_manager.h"
#include "gl_bitmap_font.h"
#include "gl_building_sprite.h"
#include "gl_decoration_sprite.h"
#include "cursor_image.h"
#include "gl_circle.h"
#include "gl_rectangle.h"
#include "image_sprite.h"
#include "gl_unit_sprite.h"
#include "terrain.h"
#include "grid.h"
#include "minimap_rectangle.h"

#include <game/game.h>
#include <engine.h>

using namespace std;
using namespace glm;

namespace GLItems {

	namespace
	{
		glBitmapFont bitmapFont;
		glBuildingSprite buildingSprite;
		CursorImage cursorImage;
		glDecorationSprite decorationSprite;
		glCircle glcircle;
		EmptyRectangle emptyRectangle;
		FilledRectangle filledRectangle;
		Grid grid;
		ImageSprite imageSprite;
		MinimapRectangle minimapRectangle;
		Terrain terrain;
		glUnitSprite unitSprite;

		mat4 MENU_PROJECTION_MATRIX = mat4(0);
		mat4 CAMERA_PROJECTION_MATRIX = mat4(0);
		mat4 MINIMAP_PROJECTION_MATRIX = mat4(0);
	};

	AudioManager *Audio() { return &audioM; }
	glBitmapFont *Text() { return &bitmapFont; }
	glBuildingSprite *BSprite() { return &buildingSprite; }
	glDecorationSprite *DSprite() { return &decorationSprite; }
	CursorImage *Cursor() { return &cursorImage; }
	glCircle *ECircle() { return &glcircle; }
	EmptyRectangle *ERectangle() { return &emptyRectangle; }
	FilledRectangle *FRectangle() { return &filledRectangle; }
	ImageSprite *Img() { return &imageSprite; }
	glUnitSprite *USprite() { return &unitSprite; }
	Terrain *MapTerrain() { return &terrain; }
	Grid *MapGrid() { return &grid; }
	MinimapRectangle *MMRectangle() { return &minimapRectangle; }

	mat4 getCameraProjectionMatrix(void)
	{ 
		return CAMERA_PROJECTION_MATRIX; 
	}

	mat4 getMinimapProjectionMatrix(void) 
	{ 
		return MINIMAP_PROJECTION_MATRIX; 
	}

	void setMenuProjectionMatrix(mat4 mat)
	{
		MENU_PROJECTION_MATRIX = mat;
	}

	void setCameraProjectionMatrix(mat4 mat)
	{
		CAMERA_PROJECTION_MATRIX = mat;
	}

	void setMinimapProjectionMatrix()
	{
		float bottom = (-1.f) * (MEDIUM_MAP_HEIGHT * Engine::myWindow::BottomBarHeight / (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight));
		float top = MEDIUM_MAP_HEIGHT + MEDIUM_MAP_HEIGHT * Engine::myWindow::TopBarHeight / (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight);
		float left = 0.f;
		float right = (float)MEDIUM_MAP_WIDTH;
		MINIMAP_PROJECTION_MATRIX = ortho(left, right, bottom, top, -right, right);
	}


	void init() {
		*Text() = glBitmapFont();
		*BSprite() = glBuildingSprite();
		*DSprite() = glDecorationSprite();
		*Cursor() = CursorImage();
		*ECircle() = glCircle();
		*ERectangle() = EmptyRectangle();
		*FRectangle() = FilledRectangle();
		*Img() = ImageSprite();
		*USprite() = glUnitSprite();
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
		Text()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
		Cursor()->apply_projection_matrix(MENU_PROJECTION_MATRIX);

		//----

		Text()->create();
		Cursor()->create();
		ECircle()->create();

		ERectangle()->create();
		FRectangle()->create();
		
		BSprite()->Create();
		DSprite()->Create();
		USprite()->Create();

		Img()->create();

		//----

		MapTerrain()->create();
		MapGrid()->create();
		MMRectangle()->create();
	}

	void applyMenuMatrices() {
		Img()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
		Img()->apply_view_matrix();
		ECircle()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
		ECircle()->apply_view_matrix();
		ERectangle()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
		ERectangle()->apply_view_matrix();
		FRectangle()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
		FRectangle()->apply_view_matrix();
		MMRectangle()->apply_projection_matrix(MENU_PROJECTION_MATRIX);
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

};