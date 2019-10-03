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
	
	std::string s;

	for (int a = 0; a < menus.size(); ++a) {

		std::ifstream path("assets/data/interface/menu/" + menus[a] + ".json");
		data = json::parse(path);
		
		/* temporary objects */
		s = "images";
		gui::Image img;
		gui::Button btn;

		for (int i = 0; i < data[s].size(); ++i) {

			/* read data from json */
			std::string imageName = data[s][i]["image_name"].get<std::string>();
			std::string name = data[s][i]["name"].get<std::string>();
			std::string size = data[s][i]["size"].get<std::string>();
			std::string align = data[s][i]["align"].get<std::string>();
			int clickable = data[s][i]["clickable"].get<int>();
			int x = data[s][i]["x"].get<int>();
			int y = data[s][i]["y"].get<int>();

			/* use data */
			img = gui::Image(imageName);	
			if (size == "auto") img.create(align, x, y, 0, 0, pickingId * clickable);
			if (size == "max") img.create(align, x, y, GLB::WINDOW_WIDTH, GLB::WINDOW_HEIGHT, pickingId * clickable);
			images[menus[a]].push_back(img);

			// update picking 
			if (clickable) {
				pickingList[pickingId] = name;
				pickingId ++;
			}
		}

		s = "buttons";
		for (int i = 0; i < data[s].size(); ++i) {

			/* read data from json */
			std::string imageName = data[s][i]["image_name"].get<std::string>();
			std::string jsonText = data[s][i]["text"].get<std::string>();
			int x = data[s][i]["x"].get<int>();
			int y = data[s][i]["y"].get<int>();

			/* use data */
			btn = gui::Button();
			btn.create(imageName, jsonText, x, y, pickingId, glm::vec4(0.f, 0.f, 0.f, 255.f));
			buttons[menus[a]].push_back(btn);

			// update picking 
			pickingList[pickingId] = data[s][i]["name"].get<std::string>();
			pickingId++;
		}

		s = "playersList";
		if (data[s].size()>0) {
			list = PlayersList();
			list.create(data[s]["x"], data[s]["y"], &pickingList, &pickingId, &players_color);
		}
	}
	menuIsCreated = true;
	obj::applyMenuMatrices();
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

		std::cout << clickId << "\n";

		/*------------------------------------------------------------------------------*/
		if (currentMenu == "singleplayer"){
			if (pickingList[clickId].substr(0, 4) != "CivForm") {
				list.close();
			}
			list.picking(pickingList, &num_players, &players_color, clickId);
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonStart") {
			
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
		if (pickingList[clickId] == "buttonEditor") {
			GLB::MOUSE_LEFT = false;
			GLB::EDITOR = true;
			GLB::MAIN_MENU = false;
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
