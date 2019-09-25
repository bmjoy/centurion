#pragma once

#include <global.hpp>
#include <objects.h>
#include "../interface/game_ui_menu.h"
#include "../gui/rectangle.h"

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
	gui::Rectangle top_bar;
	gui::Rectangle bottom_bar;
	GameMenu gameMenu;

	void picking();
};

