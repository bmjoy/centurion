#include <engine>

using namespace glb;

CPoint::CPoint(){}

void CPoint::create() {
	img = gui::Image("circle_pos");
	img.create("center", 0.f, 0.f, 0, 0, 0);	
}
void CPoint::render() {
	if (!GAME::MINIMAP_IS_ACTIVE){
		img.render(false, getParam("mouse-x-position"), getParam("mouse-y-2D-position"));
	}
}

CPoint::~CPoint()
{
}
