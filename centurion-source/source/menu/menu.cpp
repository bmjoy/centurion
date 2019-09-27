#include "menu.h"

Menu::Menu(){
	currentMenu = "mainmenu";
	menus = { "mainmenu", "singleplayer" };
	pickingId = 1;
	pickingList[0] = "background";
	images = { };
	buttons = { };
	pickingList = { };
	num_players = 2;
	menuIsCreated = false;
}

void Menu::create(std::vector<Player> *List) {	
	playersList = List;

	music = Music().play("assets/music/menu.mp3", true);

	for (int a = 0; a < menus.size(); ++a) {

		std::ifstream path("assets/data/interface/" + menus[a] + ".json");
		data = json::parse(path);
		s = "images";
		
		for (int i = 0; i < data[s].size(); ++i) {

			/* read data from json */
			std::string imageName = data[s][i]["image_name"].get<std::string>();
			std::string name = data[s][i]["name"].get<std::string>();
			std::string size = data[s][i]["size"].get<std::string>();
			std::string align = data[s][i]["align"].get<std::string>();
			int clickable = data[s][i]["clickable"].get<int>();
			int jsonX = data[s][i]["x"].get<int>();
			int jsonY = data[s][i]["y"].get<int>();

			/* use data */
			(jsonX >= 0) ? x = jsonX : x = GLB::WINDOW_WIDTH + jsonX;
			(jsonY >= 0) ? y = jsonY : y = GLB::WINDOW_HEIGHT + jsonY;

			img = gui::Image(imageName);	
			img.set_id(pickingId * clickable);
			if (size == "auto") img.create(align, x, y);
			if (size == "max") img.create(align, x, y, GLB::WINDOW_WIDTH, GLB::WINDOW_HEIGHT);
			images[menus[a]].push_back(img);

			// update picking 
			pickingList[pickingId * clickable] = name;
			pickingId += pickingId * clickable;
		}

		s = "buttons";
		for (int i = 0; i < data[s].size(); ++i) {

			/* read data from json */
			std::string imageName = data[s][i]["image_name"].get<std::string>();
			std::string jsonText = data[s][i]["text"].get<std::string>();
			int jsonX = data[s][i]["x"].get<int>();
			int jsonY = data[s][i]["y"].get<int>();

			/* use data */
			(jsonX >= 0) ? x = jsonX : x = GLB::WINDOW_WIDTH + jsonX;
			(jsonY >= 0) ? y = jsonY : y = GLB::WINDOW_HEIGHT + jsonY;

			btn = gui::Button();
			btn.set_text(jsonText);
			btn.set_id(pickingId);
			btn.create(imageName, x, y);
			buttons[menus[a]].push_back(btn);

			// update picking 
			pickingList[pickingId] = data[s][i]["name"].get<std::string>();
			pickingId++;
		}

		s = "playersList";
		if (data[s].size()>0) {
			list = PlayersList();
			list.set_position(data[s]["x"], data[s]["y"]);
			list.create(&pickingList, &pickingId, &players_color);
		}
	}
	menuIsCreated = true;
}


void Menu::render() {

	for (int i = 0; i < images[currentMenu].size(); ++i) {
		images[currentMenu][i].render(true);
	}
	for (int i = 0; i < buttons[currentMenu].size(); ++i) {
		buttons[currentMenu][i].render(true);
	}
	if (currentMenu == "singleplayer") {
		list.render(num_players, players_color, true);
	}

	picking();

	for (int i = 0; i < images[currentMenu].size(); ++i) {
		images[currentMenu][i].render(false);
	}
	for (int i = 0; i < buttons[currentMenu].size(); ++i) {
		buttons[currentMenu][i].render(false);
	}
	if (currentMenu == "singleplayer") {
		list.render(num_players, players_color, false);
	}
	
}

void Menu::picking() {

	if (GLB::MOUSE_LEFT) {

		clickId = get_id();

		/*------------------------------------------------------------------------------*/
		if (currentMenu == "singleplayer"){
			if (pickingList[clickId].substr(0, 4) != "CivForm") {
				list.close();
			}
			list.picking(pickingList, &num_players, &players_color, clickId);
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonStart") {
			Music().stop(music);
			music = Music().play("assets/music/game.mp3", true);
			GLB::MOUSE_LEFT = false;
			GLB::GAME = true;
			GLB::MAIN_MENU = false;
			/* save game informations */
			GAME::PLAYERS_NUMBER = num_players;
			for (int i = 0; i < num_players; i++) {
				Player p = Player();
				p.create(players_color[i], 0, list.get_race(i), GLB::COLORS[players_color[i]]);
				(*playersList).push_back(p);				
			}
			
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonQuit") {
			GLB::MOUSE_LEFT = false;
			GLB::WINDOW_CLOSE = true;
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonExit") {
			GLB::MOUSE_LEFT = false;
			currentMenu = "mainmenu";
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonSinglePlayer") {
			GLB::MOUSE_LEFT = false;
			currentMenu = "singleplayer";
		}
	}
}


Menu::~Menu()
{
}
