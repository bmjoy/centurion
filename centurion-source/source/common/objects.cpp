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

void obj::init() {
	*obj::Text() = CBitmapFont();
	*obj::BuildingSprite() = BSprite();
	*obj::ERectangle() = EmptyRectangle();
}

void obj::compile() {
	int id;
	id = obj::Text()->compile();
	id = obj::BuildingSprite()->compile();
	id = obj::ERectangle()->compile();
}

void obj::create() {
	obj::Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Text()->create();
	obj::BuildingSprite()->create();
	obj::ERectangle()->init();
}
