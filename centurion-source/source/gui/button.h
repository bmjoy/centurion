#pragma once

#include "ui_object.h"
#include "../primitives/image.h"
#include "../primitives/bitmap_font.h"

class Button : public UIObject
{
public:
	Button();
	void create();
	void render(bool picking);
	~Button();

private:
	CBitmapFont buttonText;
	Image buttonImage;
};
