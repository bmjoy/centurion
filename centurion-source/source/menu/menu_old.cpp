#include <menu>

#include <player/player.h>
#include <picking.h>
#include <game/strategy.h>
#include <interface>
#include <engine.h>
#include <translationsTable.h>


using namespace std;



namespace menu {

	Menu::Menu(){
		options = new GlobalOptions();
		currentMenu = "mainmenu";
		menus = { "mainmenu", "singleplayer", "options" };
		images = { };
		buttons = { };
		num_players = 2;
		menuIsCreated = false;
	}

	void Menu::create() {	
		list = new PlayersList();
		PickingObject::resetPicking();
		PickingUI::resetPicking();
	
		string s;
		for (int a = 0; a < menus.size(); ++a) {

			ifstream path("assets/data/interface/menu/" + menus[a] + ".json");
			data = json::parse(path);
		
			/* temporary objects */
			s = "images";
			gui::Image img;
			gui::Button btn;

			for (int i = 0; i < data[s].size(); ++i) {

				/* read data from json */
				string imageName = data[s][i]["image_name"].get<string>();
				string name = data[s][i]["name"].get<string>();
				string size = data[s][i]["size"].get<string>();
				string align = data[s][i]["align"].get<string>();
				int clickable = data[s][i]["clickable"].get<int>();
				int x = data[s][i]["x"].get<int>();
				int y = data[s][i]["y"].get<int>();

				/* use data */
				img = gui::Image(imageName);	
				if (size == "auto") img.create(align, (float)x, (float)y, 0.0f, 0.0f, PickingUI::obtainPickingID() * clickable);
				if (size == "max") img.create(align, (float)x, (float)y, Engine::myWindow::Width, Engine::myWindow::Height, PickingUI::obtainPickingID() * clickable);
				images[menus[a]].push_back(img);

				// update picking 
				if (clickable) {
					PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, name);
				}
			}

			s = "buttons";
			for (int i = 0; i < data[s].size(); ++i) {

				/* read data from json */
				string imageName = data[s][i]["image_name"].get<string>();
				string name = data[s][i]["name"].get<string>();
				string buttonText = TranslationsTable::GetTranslation(name);
				int x = data[s][i]["x"].get<int>();
				int y = data[s][i]["y"].get<int>();

				/* use data */
				btn = gui::Button();
				btn.create(imageName, buttonText, x, y, PickingUI::obtainPickingID(), glm::vec4(0.f, 0.f, 0.f, 255.f));
				buttons[menus[a]].push_back(btn);

				// update picking 
				PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, name);
			}

			s = "playersList";
			if (data[s].size()>0) {
				
				list->create(data[s]["x"], data[s]["y"], &players_color);
			}
		}
		options->create();
		menuIsCreated = true;
		applyMenuMatrices();
	}

	void Menu::update() {
		list = new PlayersList();
		PickingUI::resetPicking();
		PickingObject::resetPicking();

		string s;
		for (int a = 0; a < menus.size(); ++a) {

			ifstream path("assets/data/interface/menu/" + menus[a] + ".json");
			data = json::parse(path);

			/* temporary objects */
			s = "images";
			gui::Image img;
			gui::Button btn;

			for (int i = 0; i < data[s].size(); ++i) {

				/* read data from json */
				string imageName = data[s][i]["image_name"].get<string>();
				string name = data[s][i]["name"].get<string>();
				string size = data[s][i]["size"].get<string>();
				string align = data[s][i]["align"].get<string>();
				int clickable = data[s][i]["clickable"].get<int>();
				int x = data[s][i]["x"].get<int>();
				int y = data[s][i]["y"].get<int>();

				/* use data */
				img = gui::Image(imageName);
				if (size == "auto") img.create(align, (float)x, (float)y, 0.0f, 0.0f, PickingUI::obtainPickingID() * clickable);
				if (size == "max") img.create(align, (float)x, (float)y, Engine::myWindow::Width, Engine::myWindow::Height, PickingUI::obtainPickingID() * clickable);
				images[menus[a]].push_back(img);

				// update picking 
				if (clickable) {
					PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, name);
				}
			}

			s = "buttons";
			for (int i = 0; i < data[s].size(); ++i) {

				/* read data from json */
				string imageName = data[s][i]["image_name"].get<string>();
				string name = data[s][i]["name"].get<string>();
				string buttonText = TranslationsTable::GetTranslation(name);
				int x = data[s][i]["x"].get<int>();
				int y = data[s][i]["y"].get<int>();

				/* use data */
				btn = gui::Button();
				btn.create(imageName, buttonText, x, y, PickingUI::obtainPickingID(), glm::vec4(0.f, 0.f, 0.f, 255.f));
				buttons[menus[a]].push_back(btn);

				// update picking 
				PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, name);
			}

			s = "playersList";
			if (data[s].size() > 0) {

				list->create(data[s]["x"], data[s]["y"], &players_color);
			}
		}
		options->create();
	}

	void Menu::render() {

		/* picking */

		if (Engine::Mouse::LeftClick){
			for (int i = 0; i < images[currentMenu].size(); ++i) 
				images[currentMenu][i].render(true);
			
			for (int i = 0; i < buttons[currentMenu].size(); ++i) 
				buttons[currentMenu][i].render(true);
			
			if (currentMenu == "singleplayer") 
				list->render(num_players, players_color, true);
			
			if (currentMenu == "options")
				options->render(true);

			picking();
		}

		/* rendering */

		for (int i = 0; i < images[currentMenu].size(); ++i) 
			images[currentMenu][i].render(false);
		
		for (int i = 0; i < buttons[currentMenu].size(); ++i) 
			buttons[currentMenu][i].render(false);
		
		if (currentMenu == "singleplayer") 
			list->render(num_players, players_color, false);
		if (currentMenu == "options")
			options->render(false);
	
		Engine::Mouse::RightClick = false;
		Engine::Mouse::LeftClick = false;
		Engine::Mouse::RightClick = false;
	}

	void Menu::reset() {
		menuIsCreated = false;
		delete list;
	}

	Menu::~Menu(){
		delete options;
	}
}