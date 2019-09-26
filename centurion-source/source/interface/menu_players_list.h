#pragma once

#include <global.hpp>
#include <objects.h>

#include "../gui/form_input.h"
#include "../gui/simple_text.h"
#include "../gui/div_image.h"
#include "../gui/rectangle.h"

class PlayersList
{
public:
	PlayersList();
	void set_position(int startX, int startY);
	void create(std::map<int, std::string> *pickingList, int *pickingId, std::vector<int> *players_color);
	void close();
	void picking(std::map<int, std::string> pickingList, int *numPlayers, std::vector<int> *players_color, int clickId);
	void render(int numPlayers, std::vector<int> players_color, bool picking);
	std::string get_race(int i);
	~PlayersList();
private:
	std::vector<FormInput> colors_Form;
	std::vector<FormInput> players_Form;
	std::vector<FormInput> civiliz_Form;
	gui::SimpleText text;
	float textWidth;
	gui::SimpleText number;
	DivImage arrowDown, arrowUp;
	gui::Rectangle background;
	int x, y, deltaY, w, h;
};

