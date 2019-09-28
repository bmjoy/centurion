#pragma once

#include <global.hpp>
#include <json.hpp>

#include "../gui/button.h"
#include "../gui/rectangle.h"

class GameMenu
{
public:
	GameMenu();
	void create(int *pickingId, std::map<int, std::string> *pickingList);
	void render(bool picking);
	~GameMenu();
private:
	gui::Button btn;
	gui::Rectangle background;
	gui::Rectangle border;
	std::vector<gui::Button> buttons;
	json data;
	std::string s;
	float w, h, x, y;
	int clickId;
	bool gameMenuActive;
};

