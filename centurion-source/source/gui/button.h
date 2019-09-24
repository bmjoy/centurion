#pragma once

#include "ui_object.h"
#include "../primitives/image.h"
#include "div_text.h"

class Button : public UIObject
{
public:
	Button();
	void create();
	void render(bool picking);
	~Button();

private:
	DivText buttonText;
	Image buttonImage;
};
