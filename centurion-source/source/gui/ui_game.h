#pragma once

#include <global.hpp>
#include "../primitives/filled_rectangle.h"
#include "../interface/game_menu.h"
#include "div_text.h"

class UIGame
{
public:
	UIGame();
	void create(int *pickingId);
	void render(bool pick);
	~UIGame();
private:
	int *objectId, clickId;
	std::map<int, std::string> pickingList;
	FilledRectangle ui_top;
	FilledRectangle ui_bottom;
	GameMenu gameMenu;

	void picking();
};

