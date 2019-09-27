#include "objects.h"

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

void obj::init() {
	*obj::Text() = BitmapFont();
	*obj::BSprite() = BuildingSprite();
	*obj::Cursor() = CursorImage();
	*obj::ERectangle() = EmptyRectangle();
	*obj::FRectangle() = FilledRectangle();
	*obj::Img() = ImageSprite();
	*obj::USprite() = UnitSprite();
}

void obj::compile() {
	int id;
	id = obj::Text()->compile();
	id = obj::BSprite()->compile();
	id = obj::ERectangle()->compile();
	id = obj::FRectangle()->compile();
	id = obj::Cursor()->compile();
	id = obj::Img()->compile();
	id = obj::USprite()->compile();
}

void obj::create() {
	obj::Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Cursor()->apply_projection_matrix(GLB::MENU_PROJECTION);

	//----

	obj::Text()->create();
	obj::Cursor()->create();
	obj::ERectangle()->init();
	obj::FRectangle()->create();
	obj::BSprite()->create();	
	obj::Img()->create();
	obj::USprite()->create();
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
}