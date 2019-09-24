#include "players_list.h"



PlayersList::PlayersList(){
	deltaY = 30;
	w = 550;
	h = 400;
}

void PlayersList::set_position(int startX, int startY){
	x = startX; y = startY;
}

void PlayersList::create(std::map<int, std::string> *pickingList, int *pickingId, std::vector<int> *players_color) {
	text = DivText();
	
	arrowDown = DivImage();
	arrowDown.set_img_path("assets/ui/menu/down.png");
	arrowDown.set_position(glm::vec2(x + 240, y + 50));
	arrowDown.set_id(*pickingId);
	(*pickingList)[*pickingId] = "arrowDown";
	(*pickingId)++;
	arrowDown.create("center");

	arrowUp = DivImage();
	arrowUp.set_img_path("assets/ui/menu/up.png");
	arrowUp.set_position(glm::vec2(x + 280, y + 50));
	arrowUp.set_id(*pickingId);
	(*pickingList)[*pickingId] = "arrowUp";
	(*pickingId)++;
	arrowUp.create("center");

	back = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	back.create(w, h, "top-left");
	back.apply_projection_matrix(GLB::MENU_PROJECTION);
	back.apply_view_matrix();

	for (int j = 0; j < GAME::PLAYERS_NUMBER_MAX; j++) {

		//Player color rectangle
		(*pickingList)[*pickingId] = "ColForm_" + std::to_string(j);
		FormInput col_fi = FormInput(false);
		col_fi.set_position(glm::vec2(x, y - deltaY * j));
		col_fi.set_id(*pickingId);
		col_fi.create(20.f, 20.f, { "" });
		if (j < 2) { (*players_color).push_back(j); }
		else { (*players_color).push_back(-1); }
		colors_Form.push_back(col_fi);
		(*pickingId)++;

		//Player name
		FormInput p_fi = FormInput(false);
		p_fi.set_position(glm::vec2(x + 50.0f, y - deltaY * j));
		p_fi.create(200.f, 20.f, { "Player " + std::to_string(j + 1) });
		players_Form.push_back(p_fi);


		//Player civilization
		(*pickingList)[*pickingId] = "CivForm_" + std::to_string(j);
		FormInput c_fi = FormInput(true);
		c_fi.set_position(glm::vec2(x + 280.f, y - deltaY * j));
		c_fi.set_id(*pickingId);
		c_fi.create(150.f, 20.f, GAME::RACES);
		civiliz_Form.push_back(c_fi);
		(*pickingId)++;
	}
}

void PlayersList::render(int numPlayers, std::vector<int> players_color, bool picking) {

	if (picking) {
		arrowDown.render(true);
		arrowUp.render(true);
		for (int j = numPlayers - 1; j >= 0; j--) {
			colors_Form[j].render(true);
			civiliz_Form[j].render(true);
		}
	}
	else {

		back.set_color(glm::vec4(0.f,0.f,0.f,0.5f));
		back.render(x-30, y+80);

		obj::ERectangle()->create(getCoords(x - 30, y + 80, w, h));
		obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f), glm::vec4(255.f));

		arrowDown.render(false);
		arrowUp.render(false);

		for (int j = numPlayers - 1; j >= 0; j--) {
			text.set_text("Number of players: " + std::to_string(numPlayers));
			text.set_position(glm::vec2(x + 1.f, y + 39.f));
			text.render("tahoma_8", glm::vec4(0.f, 0.f, 0.f, 255.f), "left", "normal");
			text.set_position(glm::vec2(x, y + 40.f));
			text.render("tahoma_8", glm::vec4(255.f), "left", "normal");
			colors_Form[j].set_color(glm::vec4(GLB::COLORS[players_color[j]], 1.0f));
			colors_Form[j].render();
			players_Form[j].render();
			civiliz_Form[j].render();
		}
	}
}

std::string PlayersList::get_race(int i) {
	return civiliz_Form[i].selectedText;
}

void PlayersList::close() {
	for (int k = 0; k < GAME::PLAYERS_NUMBER_MAX; k++) {
		civiliz_Form[k].close();
	}
}

void PlayersList::picking(std::map<int, std::string> pickingList, int *numPlayers, std::vector<int> *players_color, int clickId) {
	
	if (pickingList[clickId] == "arrowDown") {
		GLB::MOUSE_LEFT = false;
		if (*numPlayers > 2 && *numPlayers <= 8) {
			(*players_color)[*numPlayers - 1] = -1;
			(*numPlayers) -= 1;
		}
	}
	
	if (pickingList[clickId] == "arrowUp") {
		GLB::MOUSE_LEFT = false;
		if (*numPlayers >= 2 && *numPlayers < 8) {
			(*numPlayers) += 1;
			(*players_color)[*numPlayers - 1] = 0;
			bool c = false;
			while (!c) {
				int s = 0;
				for (int k = 0; k < (*players_color).size(); k++) {
					if ((*players_color)[k] == (*players_color)[*numPlayers - 1]) {
						s++;
					}
				}
				if (s == 1) {
					c = true;
				}
				else {
					(*players_color)[*numPlayers - 1] ++;
				}
			}
		}
	}
	
	
	for (int j = 0; j < *numPlayers; j++) {
		if (pickingList[clickId] == "CivForm_" + std::to_string(j)) {
			int i = int(((float)GLB::MOUSE_LEFT_Y - y + deltaY * j) / 20.0)*(-1);
			GLB::MOUSE_LEFT = false;
			civiliz_Form[j].open_close();

			if (i > 0) {
				civiliz_Form[j].select_option(i);
			}
		}
		if (pickingList[clickId] == "ColForm_" + std::to_string(j)) {
			GLB::MOUSE_LEFT = false;
			(*players_color)[j] ++;
			if ((*players_color)[j] == GLB::COLORS.size()) { (*players_color)[j] = 0; }
			bool c = false;
			while (!c) {
				int s = 0;
				for (int k = 0; k < (*players_color).size(); k++) {
					if ((*players_color)[k] == (*players_color)[j]) {
						s++;
					}
				}
				if (s == 1) {
					c = true;
				}
				else {
					(*players_color)[j] ++;
				}
			}
		}
	}

}

PlayersList::~PlayersList()
{
}
