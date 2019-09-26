#pragma once

#include "ui_object.h"
#include "../primitives/image.h"
#include "simple_text.h"

class Button : public UIObject
{
public:
	Button();
	void create();
	void render(bool picking);
	~Button();

private:
	gui::SimpleText buttonText;
	Image buttonImage;
};
