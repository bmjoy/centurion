#pragma once

#include <global.hpp>
#include "../primitives/filled_rectangle.h"
#include "div_text.h"

class UIGame
{
public:
	UIGame();
	void create();
	void render();
	~UIGame();
private:
	FilledRectangle ui_top;
	FilledRectangle ui_bottom;
};

