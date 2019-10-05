#include "game_ui_menu.h"

using namespace glb;

GameMenu::GameMenu(){
	w = 300.f;
	h = 400.f;
	gameMenuActive = false;
}

void GameMenu::create(int *pickingId, std::map<int, std::string> *pickingList) {

	std::ifstream path("assets/data/interface/game/menu.json");
	if (!path.good()) {
		forceGameClosure("Error code 0x00000001\n\n  Unable to find or process MENU DATA file.\n  Forced application shutdown has started.");
	}
	data = json::parse(path);

	x = getParam("window-width") / 2.f;
	y = getParam("window-height") / 2.f;

	s = "buttons";
	for (int i = 0; i < data[s].size(); ++i) {
		btn = gui::Button();
		btn.create(
			data[s][i]["image_name"].get<std::string>(), 
			data[s][i]["text"].get<std::string>(), 
			(int)x + data[s][i]["x"].get<int>(),
			(int)y + data[s][i]["y"].get<int>(),
			*pickingId, 
			glm::vec4(0.f, 0.f, 0.f, 255.f)
		);
		buttons.push_back(btn);

		/* update picking */
		(*pickingList)[*pickingId] = data[s][i]["name"].get<std::string>();
		(*pickingId)++;
	}

	background = gui::Rectangle();
	background.create("border-filled", x - w / 2.f, y + h / 2.f, w, h, "top-left", 0);
}

void GameMenu::render(bool picking) {

	if (GAME::MENU_IS_ACTIVE) {

		if (picking) {
			for (int i = 0; i < buttons.size(); ++i) {
				buttons[i].render(true);
			}
		}

		else {
			background.render(glm::vec4(0.f, 0.f, 0.f, 0.5f));

			for (int i = 0; i < buttons.size(); ++i) {
				buttons[i].render(false);
			}
		}		
	}
}

GameMenu::~GameMenu(){}
