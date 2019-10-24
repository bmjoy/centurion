#include <menu>
#include <picking>
#include <engine>
#include <game>
#include <global>
#include <player>
#include <interface>

using namespace glb;
using namespace engine;
using namespace game;

namespace menu {
	void Menu::picking() {
		int clickId = get_id();
		string clickName = getPickedObjectName(clickId);
		cout << "DEBUG: Click id: " << clickId << " --> " + clickName + "\n";

		/*------------------------------------------------------------------------------*/
		if (clickName == "SINGLEPLAYER_buttonExit") {
			currentMenu = "mainmenu";
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonSinglePlayer") {
			currentMenu = "singleplayer";
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonEditor") {
			ENGINE()->setEnvironment("editor");
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonOptions") {
			currentMenu = "options";
			options->currentLan = language;
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonQuit") {
			saveLog();
			setBoolean("window-should-close", true);
		}
		/*------------------------------------------------------------------------------*/
		if (currentMenu == "singleplayer") {
			if (clickName.substr(0, 4) != "CivForm")
				list->close();
			list->picking(&num_players, &players_color, clickId);
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "SINGLEPLAYER_buttonStart") {
			setBoolean("mouse-left", false);
			ENGINE()->setEnvironment("game");
			/* save game informations */
			game::playersNumber = num_players;
			for (int i = 0; i < num_players; i++) {
				Player p = Player();
				p.create(players_color[i], 0, list->get_race(i), glb::colors[players_color[i]]);
				playersList[i] = p;
			}
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "OPTIONS_buttonSave") {
			changeLanguage(options->currentLan);
			currentMenu = "mainmenu";
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "OPTIONS_buttonCancel") {
			currentMenu = "mainmenu";
		}
	}
};