#pragma once

#include <global.hpp>
#include <objects.h>

#include "../gui/form_input.h"
#include "../gui/simple_text.h"
#include "../gui/image.h"
#include "../gui/rectangle.h"

class PlayersList
{
public:
	PlayersList();
	void create(int startX, int startY, std::map<int, std::string> *pickingList, int *pickingId, std::vector<int> *players_color);
	void close();
	void picking(std::map<int, std::string> pickingList, int *numPlayers, std::vector<int> *players_color, int clickId);
	void render(int numPlayers, std::vector<int> players_color, bool picking);
	std::string get_race(int i);
	~PlayersList();
private:
	std::vector<gui::FormInput> colors_Form;
	std::vector<gui::FormInput> players_Form;
	std::vector<gui::FormInput> civiliz_Form;
	gui::SimpleText text;
	gui::SimpleText number;
	gui::Image arrowDown, arrowUp;
	gui::Rectangle background;
	gui::Rectangle border;
	float textWidth;
	int x, y, deltaY, w, h;
};

