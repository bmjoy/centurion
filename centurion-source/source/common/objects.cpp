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
	obj::Text()->create();
	obj::Cursor()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Cursor()->create();
	obj::ERectangle()->init();
	obj::FRectangle()->create();
	obj::BSprite()->create();
	obj::Img()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Img()->apply_view_matrix();
	obj::Img()->create();
	obj::USprite()->create();
}
