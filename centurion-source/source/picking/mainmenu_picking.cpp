#include <menu>
#include <picking.h>
#include <engine/engine.h>
#include <engine/window.h>
#include <engine/mouse.h>
#include <settings.h>
#include <game/strategy.h>
#include <global>
#include <player>
#include <interface>

using namespace glb;
using namespace engine;


namespace menu {
	void Menu::picking() {
		int clickId = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(clickId);
		stringstream ss;

		ss << "Click id: " << clickId << " --> " + clickName;
		Logger::Info(ss.str());

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
			Engine::setEnvironment("editor");
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonOptions") {
			currentMenu = "options";
			options->currentLan = Settings::Language;
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "MAINMENU_buttonQuit") {
			Settings::SaveXml();
			engine::myWindow::ShouldClose = true;
		}
		/*------------------------------------------------------------------------------*/
		if (currentMenu == "singleplayer") {
			if (clickName.substr(0, 4) != "CivForm")
				list->close();
			list->picking(&num_players, &players_color, clickId);
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "SINGLEPLAYER_buttonStart") {
			Mouse::LeftClick = false;
			Engine::setEnvironment("game");
			/* save game informations */
			Game::SetNumberOfPlayers(num_players);
			for (int i = 0; i < num_players; i++) {
				Player p = Player();
				p.create(players_color[i], 0, list->get_race(i), glb::colors[players_color[i]]);
				playersList[i] = p;
			}
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "OPTIONS_buttonApply") {
			if (Settings::Language != options->currentLan)
				changeLanguage(options->currentLan);
			Settings::SaveXml();
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "OPTIONS_buttonSave") {
			if (Settings::Language != options->currentLan)
				changeLanguage(options->currentLan);
			currentMenu = "mainmenu";
			Settings::SaveXml();
		}
		/*------------------------------------------------------------------------------*/
		if (clickName == "OPTIONS_buttonCancel") {
			currentMenu = "mainmenu";
		}
	}
};