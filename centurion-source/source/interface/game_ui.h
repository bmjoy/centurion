#pragma once

#include <global>
#include "../interface/game_ui_menu.h"
#include <gui>

class UIGame
{
public:
	UIGame();
	void create();
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

	gui::Rectangle top_bar;
	gui::Rectangle bottom_bar;
	gui::Rectangle minimapRectangle;
	GameMenu gameMenu;

	void picking();
	void calculateTime();
};

