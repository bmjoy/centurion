#include "objects.h"

CBitmapFont *obj::Text(){ 
	return &obj::txt; 
}

BSprite *obj::BuildingSprite(){ 
	return &obj::bsprite; 
}

void obj::init() {
	*obj::Text() = CBitmapFont();
	*obj::BuildingSprite() = BSprite();
}

void obj::compile() {
	int id;
	id = obj::Text()->compile();
	id = obj::BuildingSprite()->compile();
}

void obj::create() {
	obj::Text()->create();
	obj::BuildingSprite()->create();
}
