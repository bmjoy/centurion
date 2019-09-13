#pragma once

#include <global.hpp>

#include "ui_object.h"
#include "../primitives/bitmap_font.h"

class DivText : public UIObject
{
public:
	DivText();
	void set_align(std::string halign = "left", std::string valign = "normal");
	void create(std::string font);
	void render(glm::vec4 color = glm::vec4(255.f));
	~DivText();
private:
	CBitmapFont txt;
};

