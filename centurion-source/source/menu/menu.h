#pragma once

#include <global.hpp>
#include <json.hpp>

#include <gui>
#include "../interface/menu_players_list.h"
#include "../player/player.h"

class Menu
{
public:
	Menu();
	bool menu_is_created() { return menuIsCreated; }
	void reset() { menuIsCreated = false; }
	void create(std::vector<Player> *List);
	void render();
	~Menu();

private:
	json data;
	std::string currentMenu;
	std::vector<std::string> menus;

	std::map<std::string, std::vector<gui::Image>> images;
	std::map<std::string, std::vector<gui::Button>> buttons;
	
	PlayersList list;

	/* PICKING */
	std::map<int, std::string> pickingList;
	int pickingId, clickId;
	void picking();

	/* INFORMATIONS */

	int num_players;
	std::vector<int> players_color;
	std::vector<Player> *playersList;
	bool menuIsCreated;

};

