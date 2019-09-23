#pragma once

#include <global.hpp>
#include "../gui/form_input.h"
#include "../primitives/empty_rectangle.h"
#include "../primitives/filled_rectangle.h"
#include "../gui/div_text.h"
#include "../gui/div_image.h"

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
	DivText text;
	DivImage arrowDown, arrowUp;
	EmptyRectangle border;
	FilledRectangle back;
	int x, y, deltaY, w, h;
};

