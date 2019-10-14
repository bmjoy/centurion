#include <menu>
#include <global>
#include <player>
#include <picking>
#include <engine>
#include <game>

#include "../interface/menu_players_list.h"

using namespace glb;
using namespace std;
using namespace engine;
using namespace game;

namespace menu {

	Menu::Menu(){
		list = new PlayersList();
		currentMenu = "mainmenu";
		menus = { "mainmenu", "singleplayer" };
		images = { };
		buttons = { };
		num_players = 2;
		menuIsCreated = false;
	}

	void Menu::create() {	
	
		resetPicking();
	
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
				if (size == "auto") img.create(align, (float)x, (float)y, 0.0f, 0.0f, getPickingID() * clickable);
				if (size == "max") img.create(align, (float)x, (float)y, getParam("window-width"), getParam("window-height"), pickingID * clickable);
				images[menus[a]].push_back(img);

				// update picking 
				if (clickable) {
					addValueToPickingListUI(getPickingID(), name);
					increasePickingID();
				}
			}

			s = "buttons";
			for (int i = 0; i < data[s].size(); ++i) {

				/* read data from json */
				string imageName = data[s][i]["image_name"].get<string>();
				string name = data[s][i]["name"].get<string>();
				string buttonText = getTranslation(name);
				int x = data[s][i]["x"].get<int>();
				int y = data[s][i]["y"].get<int>();

				/* use data */
				btn = gui::Button();
				btn.create(imageName, buttonText, x, y, getPickingID(), glm::vec4(0.f, 0.f, 0.f, 255.f));
				buttons[menus[a]].push_back(btn);

				// update picking 
				addValueToPickingListUI(getPickingID(), name);
				increasePickingID();
			}

			s = "playersList";
			if (data[s].size()>0) {
				
				list->create(data[s]["x"], data[s]["y"], &players_color);
			}
		}
		menuIsCreated = true;
		obj::applyMenuMatrices();
	}


	void Menu::render() {

		/* picking */

		if (getBoolean("mouse-left")){
			for (int i = 0; i < images[currentMenu].size(); ++i) {
				images[currentMenu][i].render(true);
			}
			for (int i = 0; i < buttons[currentMenu].size(); ++i) {
				buttons[currentMenu][i].render(true);
			}
			if (currentMenu == "singleplayer") {
				list->render(num_players, players_color, true);
			}
			picking();
		}

		/* rendering */

		for (int i = 0; i < images[currentMenu].size(); ++i) {
			images[currentMenu][i].render(false);
		}
		for (int i = 0; i < buttons[currentMenu].size(); ++i) {
			buttons[currentMenu][i].render(false);
		}
		if (currentMenu == "singleplayer") {
			list->render(num_players, players_color, false);
		}
	
		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false); 
	}

	Menu::~Menu(){
		delete list;
	}

}