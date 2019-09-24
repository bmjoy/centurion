#pragma once

#include <global.hpp>
#include <objects.h>

#include "ui_object.h"
#include "../primitives/bitmap_font.h"

class DivText : public UIObject
{
public:
	DivText();
	void render(std::string Font, glm::vec4 color, std::string halign, std::string valign);
	~DivText();
private:
	//CBitmapFont txt;
	std::string font;
};

