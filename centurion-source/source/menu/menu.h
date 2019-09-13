#pragma once

#include <global.hpp>

#include "../music/music.h"
#include "../gui/form_input.h"
#include "../gui/button.h"
#include "../gui/div_image.h"
#include "../gui/div_text.h"


class Menu
{
public:
	Menu();
	void create();
	void render();
	~Menu();

private:
	/*-------------------------------------------------------*/

	/* OBJECTS */

	DivImage background;
	DivImage arrowUp, arrowDown;
	Button buttonStart;
	Button buttonQuit;
	DivText text;
	ISoundEngine* music;
	std::vector<FormInput> colors_Form;
	std::vector<FormInput> players_Form;
	std::vector<FormInput> civiliz_Form;

	/*-------------------------------------------------------*/

	/* PICKING */

	// functions
	void picking();

	// ids
	std::string picked_name;
	int picking_id;

	// list
	std::vector<std::string> pickingIdList;

	/*-------------------------------------------------------*/

	/* SETUP VARIABLES */

	float start_x, start_y, delta_y;
	bool c;
	int s;

	/*-------------------------------------------------------*/

	/* INFORMATIONS */

	int num_players;	
	std::vector<int> players_color;
};

