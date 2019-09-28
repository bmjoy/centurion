#include "cursor_point.h"

CPoint::CPoint()
{
}

void CPoint::create() {
	img = gui::Image("circle_pos");
	img.create("center", 0.f, 0.f, 0, 0, 0);	
}

void CPoint::render() {
	if (!GAME::MINIMAP_IS_ACTIVE){
		img.render(false, GLB::MOUSE_X, GLB::MOUSE_Y_2D);
	}
}

CPoint::~CPoint()
{
}
