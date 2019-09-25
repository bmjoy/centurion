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
