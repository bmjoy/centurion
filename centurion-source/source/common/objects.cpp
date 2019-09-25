#include "objects.h"

CBitmapFont *obj::Text(){ 
	return &obj::txt; 
}

BSprite *obj::BuildingSprite(){ 
	return &obj::bsprite; 
}

EmptyRectangle *obj::ERectangle() {
	return &obj::eRect;
}

FilledRectangle *obj::FRectangle() {
	return &obj::fRect;
}

void obj::init() {
	*obj::Text() = CBitmapFont();
	*obj::BuildingSprite() = BSprite();
	*obj::ERectangle() = EmptyRectangle();
	*obj::FRectangle() = FilledRectangle();
}

void obj::compile() {
	int id;
	id = obj::Text()->compile();
	id = obj::BuildingSprite()->compile();
	id = obj::ERectangle()->compile();
	id = obj::FRectangle()->compile();
}

void obj::create() {
	obj::Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Text()->create();
	obj::ERectangle()->init();
	obj::FRectangle()->init();
	obj::BuildingSprite()->create();
}
