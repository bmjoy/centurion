#include "div_text.h"

DivText::DivText()
{
}

void DivText::render(std::string Font, glm::vec4 color, std::string halign, std::string valign) {
	obj::Text()->apply_projection_matrix(GLB::MENU_PROJECTION);
	obj::Text()->set_align(halign, valign);	
	obj::Text()->render(Font, position.x, position.y, text, color);
}

DivText::~DivText()
{
}
