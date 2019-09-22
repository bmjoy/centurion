#pragma once

#include <global.hpp>
#include "form_input.h"
#include "../primitives/empty_rectangle.h"
#include "../primitives/filled_rectangle.h"
#include "div_text.h"
#include "div_image.h"

class PlayersList
{
public:
	PlayersList();
	void set_position(int startX, int startY);
	void create(std::map<int, std::string> *pickingList, int *pickingId, std::vector<int> *players_color);
	void close();
	void picking(std::map<int, std::string> pickingList, int *numPlayers, std::vector<int> *players_color, int clickId);
	void render(int numPlayers, std::vector<int> players_color, bool picking);
	std::vector<std::string> get_races(int num_players);
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

