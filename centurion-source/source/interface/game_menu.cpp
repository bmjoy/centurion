#include "game_menu.h"



GameMenu::GameMenu(){

	w = 300.f;
	h = 400.f;
	gameMenuActive = false;
}

void GameMenu::create(int *pickingId, std::map<int, std::string> *pickingList) {

	std::ifstream path("assets/data/interface/gamemenu.json");
	data = json::parse(path);

	x = GLB::WINDOW_WIDTH / 2.f;
	y = GLB::WINDOW_HEIGHT / 2.f;

	back = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	back.create(w, h, "top-left");

	border = EmptyRectangle(SHD::E_RECTANGLE_SHADER_ID);
	border.init();
	border.create(getCoords(x - w/2.f, y + h/2.f, w, h));

	s = "buttons";
	for (int i = 0; i < data[s].size(); ++i) {
		btn = Button();
		btn.set_img_path(data[s][i]["path"].get<std::string>());
		btn.set_position(glm::vec2(x + data[s][i]["x"].get<int>(), y + data[s][i]["y"].get<int>()));
		btn.set_text(data[s][i]["text"].get<std::string>());
		btn.set_id(*pickingId);

		/* picking */
		(*pickingList)[*pickingId] = data[s][i]["name"].get<std::string>();
		(*pickingId)++;

		btn.create();
		buttons.push_back(btn);
	}
}

void GameMenu::render(bool picking) {

	if (GAME::MENU_IS_ACTIVE) {

		if (picking) {
			for (int i = 0; i < buttons.size(); ++i) {
				buttons[i].render(true);
			}
		}

		else {
			for (int i = 0; i < buttons.size(); ++i) {
				buttons[i].render(true);
			}

			back.set_color(glm::vec4(0.f, 0.f, 0.f, 0.5f));
			back.apply_projection_matrix(GLB::MENU_PROJECTION);
			back.apply_view_matrix();
			back.render(x - w / 2.f, y + h / 2.f);
			border.apply_projection_matrix(GLB::MENU_PROJECTION);
			border.apply_view_matrix();
			border.render(glm::mat4(1.0f), glm::mat4(1.0f), glm::vec4(255.f));

			for (int i = 0; i < buttons.size(); ++i) {
				buttons[i].render(false);
			}
		}		
	}
}

GameMenu::~GameMenu(){}
