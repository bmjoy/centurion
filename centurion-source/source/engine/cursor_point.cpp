#include "cursor_point.h"

CPoint::CPoint()
{
}

void CPoint::create() {
	img = Image(SHD::IMAGE_SHADER_ID);
	img.create("assets/ui/mouse/cursor_point.png", "center");	
}

void CPoint::render() {
	if (!GAME::MINIMAP_IS_ACTIVE){
		img.apply_projection_matrix(GLB::MENU_PROJECTION);
		img.apply_view_matrix();
		img.render(GLB::MOUSE_X, GLB::MOUSE_Y_2D);
	}
}

CPoint::~CPoint()
{
}
