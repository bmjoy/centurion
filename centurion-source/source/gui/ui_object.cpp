#include "ui_object.h"

UIObject::UIObject()
{
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    picking_id = 0;
}

void UIObject::set_position(glm::vec2 pos) {
	position = glm::vec3(pos,0.0f);
}

void UIObject::set_img_path(std::string path) {
	img_path = path;
}

void UIObject::set_id(int pickingId) {
	picking_id = pickingId;
	int r = (pickingId & 0x000000FF) >> 0;
	int g = (pickingId & 0x0000FF00) >> 8;
	int b = (pickingId & 0x00FF0000) >> 16;
	pickingColor = glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}

void UIObject::set_color(glm::vec4 col)
{
	color = col;
}

void UIObject::set_text(std::string txt) {
	text = txt;
}

UIObject::~UIObject()
{
}
