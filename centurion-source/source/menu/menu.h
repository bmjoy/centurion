#pragma once

#include <global.hpp>
#include <json.hpp>

#include "../music/music.h"
#include "../gui/form_input.h"
#include "../gui/button.h"
#include "../gui/div_image.h"
#include "../gui/div_text.h"
#include "../gui/players_list.h"

class Menu
{
public:
	Menu();
	void create();
	void render();
	~Menu();

private:
	json data;
	std::string currentMenu;
	std::vector<std::string> menus;

	std::map<std::string, std::vector<DivImage>> images;
	std::map<std::string, std::vector<Button>> buttons;
	std::map<std::string, std::vector<FormInput>> formInputs;
	int x, y, w, h;
	std::string s;
	ISoundEngine* music;

	/* temporary objects */
	DivImage img;
	Button btn;
	PlayersList list;

	/* PICKING */
	std::map<int, std::string> pickingList;
	int pickingId, clickId;
	void picking();

	/* INFORMATIONS */

	int num_players;
	std::vector<int> players_color;

};

