#include "objects.h"

AudioManager *obj::Audio() {
	return &obj::audioM;
}

BitmapFont *obj::Text(){
	return &obj::txt; 
}

BuildingSprite *obj::BSprite(){
	return &obj::bsprite; 
}

CursorImage *obj::Cursor() {
	return &obj::cursor;
}

EmptyRectangle *obj::ERectangle() {
	return &obj::eRect;
}

FilledRectangle *obj::FRectangle() {
	return &obj::fRect;
}

ImageSprite *obj::Img() {
	return &obj::img;
}

UnitSprite *obj::USprite() {
	return &obj::usprite;
}

Terrain *obj::MapTerrain() {
	return &obj::terrain;
}

Grid *obj::MapGrid() {
	return &obj::grid;
}

void obj::init() {
	*obj::Audio() = AudioManager();
	*obj::Text() = BitmapFont();
	*obj::BSprite() = BuildingSprite();
	*obj::Cursor() = CursorImage();
	*obj::ERectangle() = EmptyRectangle();
	*obj::FRectangle() = FilledRectangle();
	*obj::Img() = ImageSprite();
	*obj::USprite() = UnitSprite();
	*obj::MapTerrain() = Terrain();
	*obj::MapGrid() = Grid();
}

void obj::compile() {
	obj::Text()->compile();
	obj::BSprite()->compile();
	obj::ERectangle()->compile();
	obj::FRectangle()->compile();
	obj::Cursor()->compile();
	obj::Img()->compile();
	obj::USprite()->compile();
	obj::MapTerrain()->compile();
	obj::MapGrid()->compile();
}

void obj::create() {
	obj::Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Cursor()->apply_projection_matrix(GLB::MENU_PROJECTION);

	//----

	obj::Text()->create();
	obj::Cursor()->create();
	obj::ERectangle()->create();
	obj::FRectangle()->create();
	obj::BSprite()->create();	
	obj::Img()->create();
	obj::USprite()->create();

	//----

	obj::MapTerrain()->create();
	obj::MapGrid()->create();
}

void obj::applyMenuMatrices() {
	obj::Img()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Img()->apply_view_matrix();
	obj::ERectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::ERectangle()->apply_view_matrix();
	obj::FRectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::FRectangle()->apply_view_matrix();
}

void obj::applyGameMatrices(glm::mat4 *proj, glm::mat4 *view) {
	obj::BSprite()->apply_projection_matrix(*proj);
	obj::BSprite()->apply_view_matrix(*view);
	obj::USprite()->apply_projection_matrix(*proj);
	obj::USprite()->apply_view_matrix(*view);
	obj::Img()->apply_projection_matrix(*proj);
	obj::Img()->apply_view_matrix(*view);
	obj::ERectangle()->apply_projection_matrix(*proj);
	obj::ERectangle()->apply_view_matrix(*view);
	obj::FRectangle()->apply_projection_matrix(*proj);
	obj::FRectangle()->apply_view_matrix(*view);
	obj::MapTerrain()->apply_projection_matrix(*proj);
	obj::MapTerrain()->apply_view_matrix(*view);
	obj::MapGrid()->apply_projection_matrix(*proj);
	obj::MapGrid()->apply_view_matrix(*view);
}