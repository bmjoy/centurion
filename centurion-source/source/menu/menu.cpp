#include "menu.h"

Menu::Menu(){
	pickingIdList.push_back("background");
	
	picking_id = 0;
	c = false; s = 0;

	start_x = 50.0f;
	start_y = 600.0f;
	num_players = 2;
	delta_y = 30.f;
}

void Menu::create() {
	music = Music().play("assets/music/menu.mp3", true);

	background = DivImage();
	background.set_img_path("assets/ui/menu/menu_background.png");
	background.set_position(glm::vec2(0.f, 0.f));
	background.create("bottom-left", glm::ivec2(GLB::WINDOW_WIDTH,GLB::WINDOW_HEIGHT));

	buttonStart = Button();
	pickingIdList.push_back("buttonStart");
	buttonStart.set_img_path("assets/ui/buttons/button_1.png");
	buttonStart.set_position(glm::vec2(GLB::WINDOW_WIDTH - 150.0f, 100.0f));
	buttonStart.set_text("Start");
	buttonStart.set_id(pickingIdList.size() - 1);
	buttonStart.create();
	
	buttonQuit = Button();
	pickingIdList.push_back("buttonQuit");
	buttonQuit.set_img_path("assets/ui/buttons/button_1.png");
	buttonQuit.set_position(glm::vec2(GLB::WINDOW_WIDTH - 150.0f, 50.0f));
	buttonQuit.set_text("Quit");
	buttonQuit.set_id(pickingIdList.size() - 1);
	buttonQuit.create();

	text = DivText();
	text.create("tahoma_8");

	arrowDown = DivImage();
	pickingIdList.push_back("arrowDown");
	arrowDown.set_img_path("assets/ui/menu/down.png");
	arrowDown.set_position(glm::vec2(start_x + 220.f, start_y + 50.f));
	arrowDown.set_id(pickingIdList.size() - 1);
	arrowDown.create("center");

	arrowUp = DivImage();
	pickingIdList.push_back("arrowUp");
	arrowUp.set_img_path("assets/ui/menu/up.png");
	arrowUp.set_position(glm::vec2(start_x + 260.f, start_y + 50.f));
	arrowUp.set_id(pickingIdList.size() - 1);
	arrowUp.create("center");
	
	for (int j = 0; j < GAME::PLAYERS_NUMBER_MAX; j++) {

		//Player color rectangle
		pickingIdList.push_back("ColForm_" + std::to_string(j));
		FormInput col_fi = FormInput(false);
		col_fi.set_position(glm::vec2(start_x, start_y - delta_y * j));
		col_fi.set_id(pickingIdList.size() - 1);
		col_fi.create(20.f, 20.f, { "" });
		if (j < 2) { players_color.push_back(j); }
		else { players_color.push_back(-1); }
		colors_Form.push_back(col_fi);


		//Player name
		FormInput p_fi = FormInput(false);
		p_fi.set_position(glm::vec2(start_x + 50.0f, start_y - delta_y * j));
		p_fi.create(200.f, 20.f, { "Player " + std::to_string(j + 1) });
		players_Form.push_back(p_fi);


		//Player civilization
		pickingIdList.push_back("CivForm_" + std::to_string(j));
		FormInput c_fi = FormInput(true);
		c_fi.set_position(glm::vec2(start_x + 280.f, start_y - delta_y * j));
		c_fi.set_id(pickingIdList.size() - 1);		
		c_fi.create(150.f, 20.f, GAME::RACES);
		civiliz_Form.push_back(c_fi);
		
		
	}
}


void Menu::render() {
	picking();
	
	/* RENDER SECTION */

	background.render(false);
	arrowDown.render(false);
	arrowUp.render(false);
	
	buttonStart.render(false);
	buttonQuit.render(false);

	text.set_text("Number of players: " + std::to_string(num_players));
	text.set_position(glm::vec2(start_x+1.f, start_y + 39.f));
	text.render(glm::vec4(0.f,0.f,0.f,255.f));
	text.set_position(glm::vec2(start_x, start_y + 40.f));
	text.render(glm::vec4(255.f));

	for (int j = num_players-1; j >= 0; j--) {
		colors_Form[j].set_color(glm::vec4(GLB::COLORS[players_color[j]], 1.0f));
		colors_Form[j].render();
		players_Form[j].render();
		civiliz_Form[j].render();
	}
}

void Menu::picking() {
	buttonStart.render(true);
	buttonQuit.render(true);
	arrowDown.render(true);
	arrowUp.render(true);
	for (int j = num_players - 1; j >= 0; j--) {
		colors_Form[j].render(true);
		civiliz_Form[j].render(true);
	}

	if (GLB::MOUSE_LEFT) {

		picking_id = get_id();
		picked_name = pickingIdList[picking_id];

		if (picking_id < pickingIdList.size()) {
			if (picked_name.substr(0, 4) != "CivForm") {
				for (int k = 0; k < num_players; k++) {
					civiliz_Form[k].close();
				}
			}
			if (picked_name == "buttonStart") {
				Music().stop(music);
				music = Music().play("assets/music/game.mp3", true);
				GLB::MOUSE_LEFT = false;
				GLB::GAME = true;
				GLB::MAIN_MENU = false;

				/*------------------------------------------------*/
				/* save game informations */
				GAME::PLAYERS_NUMBER = num_players;
				for (int i = 0; i < num_players; i++) {
					GAME::PLAYERS_COLOR.push_back(GLB::COLORS[players_color[i]]);
					GAME::PLAYERS_RACE.push_back(civiliz_Form[i].selectedText);
				}
				/*------------------------------------------------*/
			}
			else if (picked_name == "buttonQuit") {
				GLB::MOUSE_LEFT = false;
				GLB::WINDOW_CLOSE = true;
			}
			else if (picked_name == "arrowDown") {
				GLB::MOUSE_LEFT = false;
				if (num_players > 2 && num_players <= 8) {
					players_color[num_players-1] = -1;
					num_players -= 1;
				}
			}
			else if (picked_name == "arrowUp") {
				GLB::MOUSE_LEFT = false;
				if (num_players >= 2 && num_players < 8) {
					num_players += 1;
					players_color[num_players - 1] = 0;
					c = false;
					while (!c) {
						s = 0;
						for (int k = 0; k < players_color.size(); k++) {
							if (players_color[k] == players_color[num_players - 1]) {
								s++;
							}
						}
						if (s == 1) {
							c = true;
						}
						else {	
							players_color[num_players - 1] ++;
						}
					}
				}
			}
			for (int j = 0; j < num_players; j++) {
				if (picked_name == "CivForm_" + std::to_string(j)) {
					int i = int(((float)GLB::MOUSE_LEFT_Y - start_y + delta_y * j) / 20.0)*(-1);
					GLB::MOUSE_LEFT = false;
					civiliz_Form[j].open_close();

					if (i > 0) {
						civiliz_Form[j].select_option(i);
					}
				}
				if (picked_name == "ColForm_" + std::to_string(j)) {
					GLB::MOUSE_LEFT = false;
					players_color[j] ++;
					if (players_color[j] == GLB::COLORS.size()) { players_color[j] = 0; }
					c = false;
					while(!c){
						s = 0;
						for (int k = 0; k < players_color.size(); k++) {
							if (players_color[k] == players_color[j]) {
								s++;
							}
						}
						if (s == 1) {
							c = true;
						}
						else {
							players_color[j] ++;
						}
					}
				}
			}
		}
	}

	
}






Menu::~Menu()
{
}
