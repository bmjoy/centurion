#include <interface>
#include <game/game.h>
#include <game/strategy.h>
#include <picking.h>


#include <translationsTable.h>




namespace menu{

	PlayersList::PlayersList(){
		deltaY = 30;
		w = 550;
		h = 400;
	}

	void PlayersList::create(int startX, int startY, std::vector<int> *players_color) {
	
		x = startX; y = startY;

		dynamicText = gui::SimpleText("dynamic", true);

		arrowDown = gui::Image("arrowDown");
		arrowDown.create("center", (float)x + 240, (float)y + 50, 0, 0, PickingUI::obtainPickingID());

		PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "arrowDown");
	
		arrowUp = gui::Image("arrowUp");
		arrowUp.create("center", (float)x + 280, (float)y + 50, 0, 0, PickingUI::obtainPickingID());
		PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "arrowUp");
	
		background = gui::Rectangle();
		background.create("border-filled", (float)x - 30, (float)y + 80, (float)w, (float)h, "top-left", 0);


		for (int j = 0; j < MAX_NUMBER_OF_PLAYERS; j++) {

			//Player color rectangle
			gui::FormInput col_fi = gui::FormInput(false);
			col_fi.create((float)x, (float)y - deltaY * j, 20.f, 20.f, { "" }, PickingUI::obtainPickingID());
			PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "ColForm_" + std::to_string(j));
			if (j < 2) { (*players_color).push_back(j); }
			else { (*players_color).push_back(-1); }
			colors_Form.push_back(col_fi);

			//Player name
			gui::FormInput p_fi = gui::FormInput(false);
			p_fi.create((float)x + 50.0f, (float)y - deltaY * j, 200.f, 20.f, { "WORD_player_" + std::to_string(j + 1) }, 0);
			players_Form.push_back(p_fi);

			//Player civilization
			gui::FormInput c_fi = gui::FormInput(true);
			c_fi.create((float)x + 280.f, (float)y - deltaY * j, 150.f, 20.f, Game::GetListOfRacesNames(), PickingUI::obtainPickingID());
			PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "CivForm_" + std::to_string(j));
			civiliz_Form.push_back(c_fi);
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
			dynamicText.render_dynamic(TranslationsTable::GetTranslation("WORD_playersNumber") + ": " + std::to_string(numPlayers), "tahoma_15px", x * 1.f + 5.f, y + 40.f, glm::vec4(255.f, 255.f, 255.f, 255.f), "left", "normal");
			for (int j = numPlayers - 1; j >= 0; j--) {
				colors_Form[j].render(false, glm::vec4(Game::GetColor(players_color[j]), 1.0f));
				players_Form[j].render(false, glm::vec4(0.f, 0.f, 0.f, 1.f));
				civiliz_Form[j].render(false, glm::vec4(0.f, 0.f, 0.f, 1.f));
			}
		}
	}

	std::string PlayersList::get_race(int i) {
		return civiliz_Form[i].selectedText;
	}

	void PlayersList::close() {
		for (int k = 0; k < MAX_NUMBER_OF_PLAYERS; k++) {
			civiliz_Form[k].close();
		}
	}

	void PlayersList::picking(int *numPlayers, std::vector<int> *players_color, int clickId) {
	
		std::string clickedName = PickingUI::getPickedObjectName(clickId);

		if (clickedName == "arrowDown") {
			if (*numPlayers > 2 && *numPlayers <= 8) {
				(*players_color)[*numPlayers - 1] = -1;
				(*numPlayers) -= 1;
			}
		}
	
		if (clickedName == "arrowUp") {
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
			if (clickedName == "CivForm_" + std::to_string(j)) {
				int i = civiliz_Form[j].get_clicked_option();
				civiliz_Form[j].open_close();

				if (i > 0) {
					civiliz_Form[j].select_option(i);
				}
			}
			if (clickedName == "ColForm_" + std::to_string(j)) {
				(*players_color)[j] ++;
				if ((*players_color)[j] == Game::GetNumberOfColors()) { (*players_color)[j] = 0; }
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
};