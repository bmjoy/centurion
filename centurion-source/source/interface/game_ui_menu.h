#pragma once

#include <global.hpp>
#include <json.hpp>

#include <gui>

class GameMenu
{
public:
	GameMenu();
	void create();
	void render(bool picking);
	~GameMenu();
private:
	gui::Button btn;
	gui::Rectangle background;
	std::vector<gui::Button> buttons;
	json data;
	std::string s;
	float w, h, x, y;
	bool gameMenuActive;
};

