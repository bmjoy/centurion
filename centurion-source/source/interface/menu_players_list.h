#pragma once

#include <global.hpp>
#include <gui>

class PlayersList
{
public:
	PlayersList();
	void create(int startX, int startY, std::vector<int> *players_color);
	void close();
	void picking(int *numPlayers, std::vector<int> *players_color, int clickId);
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
	float textWidth;
	int x, y, deltaY, w, h;
};

