#pragma once

#include <global.hpp>
#include <json.hpp>
#include "../primitives/filled_rectangle.h"
#include "../primitives/empty_rectangle.h"
#include "../gui/button.h"

class GameMenu
{
public:
	GameMenu();
	void create(int *pickingId, std::map<int, std::string> *pickingList);
	void render(bool picking);
	~GameMenu();
private:
	Button btn;
	EmptyRectangle border;
	FilledRectangle back;
	std::vector<Button> buttons;
	json data;
	std::string s;
	float w, h, x, y;
	int clickId;
	bool gameMenuActive;
	void picking();
};

