#include "div_text.h"

DivText::DivText()
{
}

void DivText::set_align(std::string halign, std::string valign) {
	txt.set_align(halign, valign);
}

void DivText::create(std::string font) {
	txt = CBitmapFont(SHD::FONT_SHADER_ID);
	txt.apply_projection_matrix(GLB::MENU_PROJECTION);
	txt.create(font);
}

void DivText::render(glm::vec4 color) {
	txt.render(position.x, position.y, text, color);
}

DivText::~DivText()
{
}
