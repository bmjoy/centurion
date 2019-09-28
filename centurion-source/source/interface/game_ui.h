#pragma once

#include <global.hpp>
#include <objects.h>
#include "../interface/game_ui_menu.h"
#include "../gui/rectangle.h"
#include "../gui/simple_text.h"

class UIGame
{
public:
	UIGame();
	void create(int *pickingId);
	void render();
	~UIGame();
private:
	struct Time {
		double lastTime;
		std::string seconds_str, minutes_str, hours_str;
		int seconds, minutes, hours;
		gui::SimpleText text;
		float x;
		float y;
	} time;

	int *objectId, clickId;
	std::map<int, std::string> pickingList;
	gui::Rectangle top_bar;
	gui::Rectangle bottom_bar;
	gui::Rectangle minimapRectangle;
	GameMenu gameMenu;

	void picking();
	void calculateTime();
};

