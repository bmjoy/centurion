#include <interface>
#include <json.hpp>

#include <picking>

using namespace glb;

namespace game {

	GameMenu::GameMenu(){
		w = 300.f;
		h = 400.f;
		gameMenuActive = false;
	}

	void GameMenu::create() {

		//Close the game if it wasn't able to find or process menu.json file
		std::ifstream path("assets/data/interface/game/menu.json");
		if (!path.good()) {
			forceGameClosure("NOT_FOUND" , "ERROR_menuData");
		}
		json data = json::parse(path);

		x = Settings::WindowWidth() / 2.f;
		y = Settings::WindowHeight() / 2.f;

		s = "buttons";
		for (int i = 0; i < data[s].size(); ++i) {
			btn = gui::Button();
			btn.create(
				data[s][i]["image_name"].get<std::string>(), 
				getTranslation(data[s][i]["name"].get<std::string>()), 
				(int)x + data[s][i]["x"].get<int>(),
				(int)y + data[s][i]["y"].get<int>(),
				getPickingID(), 
				glm::vec4(0.f, 0.f, 0.f, 255.f)
			);
			buttons.push_back(btn);

			/* update picking */
			addValueToPickingListUI(getPickingID(), data[s][i]["name"].get<std::string>());
			increasePickingID();
		}

		background = gui::Rectangle();
		background.create("border-filled", x - w / 2.f, y + h / 2.f, w, h, "top-left", 0);
	}

	void GameMenu::render(bool picking) {
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

	GameMenu::~GameMenu(){}
};