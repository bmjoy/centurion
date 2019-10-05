#include "menu_players_list.h"



PlayersList::PlayersList(){
	deltaY = 30;
	w = 550;
	h = 400;
}

void PlayersList::create(int startX, int startY, std::map<int, std::string> *pickingList, int *pickingId, std::vector<int> *players_color) {
	
	x = startX; y = startY;

	number = gui::SimpleText("dynamic", true);
	text = gui::SimpleText("static");
	text.create_static("Number of players: ", "tahoma_8", (float)x, (float)y + 40.f, "left", "normal", glm::vec4(255.f));
	textWidth = text.get_width();

	arrowDown = gui::Image("arrowDown");
	arrowDown.create("center", (float)x + 240, (float)y + 50, 0, 0, *pickingId);
	(*pickingList)[*pickingId] = "arrowDown";
	(*pickingId)++;
	
	arrowUp = gui::Image("arrowUp");
	arrowUp.create("center", (float)x + 280, (float)y + 50, 0, 0, *pickingId);
	(*pickingList)[*pickingId] = "arrowUp";
	(*pickingId)++;
	
	background = gui::Rectangle();
	background.create("border-filled", (float)x - 30, (float)y + 80, (float)w, (float)h, "top-left", 0);


	for (int j = 0; j < GAME::PLAYERS_NUMBER_MAX; j++) {

		//Player color rectangle
		(*pickingList)[*pickingId] = "ColForm_" + std::to_string(j);
		gui::FormInput col_fi = gui::FormInput(false);
		col_fi.create((float)x, (float)y - deltaY * j, 20.f, 20.f, { "" }, *pickingId);
		if (j < 2) { (*players_color).push_back(j); }
		else { (*players_color).push_back(-1); }
		colors_Form.push_back(col_fi);
		(*pickingId)++;

		//Player name
		gui::FormInput p_fi = gui::FormInput(false);
		p_fi.create((float)x + 50.0f, (float)y - deltaY * j, 200.f, 20.f, { "Player " + std::to_string(j + 1) }, 0);
		players_Form.push_back(p_fi);

		//Player civilization
		(*pickingList)[*pickingId] = "CivForm_" + std::to_string(j);
		gui::FormInput c_fi = gui::FormInput(true);
		c_fi.create((float)x + 280.f, (float)y - deltaY * j, 150.f, 20.f, GAME::RACES, *pickingId);
		civiliz_Form.push_back(c_fi);
		(*pickingId)++;
	}
}

void PlayersList::render(int numPlayers, std::vector<int> players_color, bool picking) {

	if (picking) {
		arrowDown.render(true);
		arrowUp.render(true);
		for (int j = numPlayers - 1; j >= 0; j--) {
			colors_Form[j].render(true, glm::vec4());
			civiliz_Form[j].render(true, glm::vec4());
		}
	}
	else {

		background.render(glm::vec4(0.f, 0.f, 0.f, 0.5f));

		arrowDown.render(false);
		arrowUp.render(false);

		text.render_static();

		number.render_dynamic(std::to_string(numPlayers), "tahoma_8", x + textWidth, y + 40.f, glm::vec4(255.f, 255.f, 255.f, 255.f), "left", "normal");

		for (int j = numPlayers - 1; j >= 0; j--) {
			colors_Form[j].render(false, glm::vec4(GLB::COLORS[players_color[j]], 1.0f));
			players_Form[j].render(false, glm::vec4(0.f, 0.f, 0.f, 1.f));
			civiliz_Form[j].render(false, glm::vec4(0.f, 0.f, 0.f, 1.f));
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
