#include "menu.h"

Menu::Menu(){
	currentMenu = "mainmenu";
	menus = { "mainmenu", "singleplayer" };
	pickingId = 1;
	pickingList[0] = "background";
	images = { };
	buttons = { };
	pickingList = { };
	start_x = 50.0f;
	start_y = 600.0f;
	num_players = 2;
	delta_y = 30.f;
}

void Menu::create() {
	
	music = Music().play("assets/music/menu.mp3", true);

	for (int a = 0; a < menus.size(); ++a) {

		std::ifstream path("assets/data/menu/" + menus[a] + ".json");
		data = json::parse(path);

		s = "images";
		for (int i = 0; i < data[s].size(); ++i) {
			img = DivImage();
			img.set_img_path(data[s][i]["path"].get<std::string>());
			(data[s][i]["x"].get<int>() >= 0) ? x = data[s][i]["x"].get<int>() : x = GLB::WINDOW_WIDTH + data[s][i]["x"].get<int>();
			(data[s][i]["y"].get<int>() >= 0) ? y = data[s][i]["y"].get<int>() : y = GLB::WINDOW_HEIGHT + data[s][i]["y"].get<int>();
			img.set_position(glm::vec2(x, y));
			
			/* picking */
			img.set_id(pickingId * data[s][i]["clickable"].get<int>());
			pickingList[pickingId * data[s][i]["clickable"].get<int>()] = data[s][i]["name"].get<std::string>();
			pickingId += pickingId * data[s][i]["clickable"].get<int>();

			if (data[s][i]["size"].get<std::string>() == "auto") {
				img.create(data[s][i]["align"].get<std::string>());
			}
			else if (data[s][i]["size"].get<std::string>() == "max") {
				img.create(data[s][i]["align"].get<std::string>(), glm::ivec2(GLB::WINDOW_WIDTH, GLB::WINDOW_HEIGHT));
			}

			images[menus[a]].push_back(img);
		}

		s = "buttons";
		for (int i = 0; i < data[s].size(); ++i) {
			btn = Button();
			btn.set_img_path(data[s][i]["path"].get<std::string>());
			(data[s][i]["x"].get<int>() >= 0) ? x = data[s][i]["x"].get<int>() : x = GLB::WINDOW_WIDTH + data[s][i]["x"].get<int>();
			(data[s][i]["y"].get<int>() >= 0) ? y = data[s][i]["y"].get<int>() : y = GLB::WINDOW_HEIGHT + data[s][i]["y"].get<int>();
			btn.set_position(glm::vec2(x, y));
			btn.set_text(data[s][i]["text"].get<std::string>());
			btn.set_id(pickingId);
			
			/* picking */
			pickingList[pickingId] = data[s][i]["name"].get<std::string>();
			pickingId++;

			btn.create();
			buttons[menus[a]].push_back(btn);
		}

		if (menus[a] == "singleplayer") {
			text = DivText();
			text.create("tahoma_8");
			for (int j = 0; j < GAME::PLAYERS_NUMBER_MAX; j++) {

				//Player color rectangle
				pickingList[pickingId] = "ColForm_" + std::to_string(j);
				FormInput col_fi = FormInput(false);
				col_fi.set_position(glm::vec2(start_x, start_y - delta_y * j));
				col_fi.set_id(pickingId);
				col_fi.create(20.f, 20.f, { "" });
				if (j < 2) { players_color.push_back(j); }
				else { players_color.push_back(-1); }
				colors_Form.push_back(col_fi);
				pickingId++;

				//Player name
				FormInput p_fi = FormInput(false);
				p_fi.set_position(glm::vec2(start_x + 50.0f, start_y - delta_y * j));
				p_fi.create(200.f, 20.f, { "Player " + std::to_string(j + 1) });
				players_Form.push_back(p_fi);


				//Player civilization
				pickingList[pickingId] = "CivForm_" + std::to_string(j);
				FormInput c_fi = FormInput(true);
				c_fi.set_position(glm::vec2(start_x + 280.f, start_y - delta_y * j));
				c_fi.set_id(pickingId);
				c_fi.create(150.f, 20.f, GAME::RACES);
				civiliz_Form.push_back(c_fi);
				pickingId++;
			}
		}

	}
}


void Menu::render() {

	for (int i = 0; i < images[currentMenu].size(); ++i) {
		images[currentMenu][i].render(true);
	}
	for (int i = 0; i < buttons[currentMenu].size(); ++i) {
		buttons[currentMenu][i].render(true);
	}
	if (currentMenu == "singleplayer") {
		for (int j = num_players - 1; j >= 0; j--) {
			colors_Form[j].render(true);
			civiliz_Form[j].render(true);
		}
	}

	picking();

	for (int i = 0; i < images[currentMenu].size(); ++i) {
		images[currentMenu][i].render(false);
	}
	for (int i = 0; i < buttons[currentMenu].size(); ++i) {
		buttons[currentMenu][i].render(false);
	}
	if (currentMenu == "singleplayer") {
		text.set_text("Number of players: " + std::to_string(num_players));
		text.set_position(glm::vec2(start_x + 1.f, start_y + 39.f));
		text.render(glm::vec4(0.f, 0.f, 0.f, 255.f));
		text.set_position(glm::vec2(start_x, start_y + 40.f));
		text.render(glm::vec4(255.f));
		for (int j = num_players - 1; j >= 0; j--) {
			colors_Form[j].set_color(glm::vec4(GLB::COLORS[players_color[j]], 1.0f));
			colors_Form[j].render();
			players_Form[j].render();
			civiliz_Form[j].render();
		}
	}

}

void Menu::picking() {

	if (GLB::MOUSE_LEFT) {

		clickId = get_id();

		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId].substr(0, 4) != "CivForm") {
			for (int k = 0; k < num_players; k++) {
				civiliz_Form[k].close();
			}
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonStart") {
			Music().stop(music);
			music = Music().play("assets/music/game.mp3", true);
			GLB::MOUSE_LEFT = false;
			GLB::GAME = true;
			GLB::MAIN_MENU = false;
			/* save game informations */
			GAME::PLAYERS_NUMBER = num_players;
			for (int i = 0; i < num_players; i++) {
				GAME::PLAYERS_COLOR.push_back(GLB::COLORS[players_color[i]]);
				GAME::PLAYERS_RACE.push_back(civiliz_Form[i].selectedText);
			}
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonQuit") {
			GLB::MOUSE_LEFT = false;
			GLB::WINDOW_CLOSE = true;
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonExit") {
			GLB::MOUSE_LEFT = false;
			currentMenu = "mainmenu";
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "buttonSinglePlayer") {
			GLB::MOUSE_LEFT = false;
			currentMenu = "singleplayer";
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "arrowDown") {
			GLB::MOUSE_LEFT = false;
			if (num_players > 2 && num_players <= 8) {
				players_color[num_players - 1] = -1;
				num_players -= 1;
			}
		}
		/*------------------------------------------------------------------------------*/
		if (pickingList[clickId] == "arrowUp") {
			GLB::MOUSE_LEFT = false;
			if (num_players >= 2 && num_players < 8) {
				num_players += 1;
				players_color[num_players - 1] = 0;
				bool c = false;
				while (!c) {
					int s = 0;
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
		/*------------------------------------------------------------------------------*/
		for (int j = 0; j < num_players; j++) {
			if (pickingList[clickId] == "CivForm_" + std::to_string(j)) {
				int i = int(((float)GLB::MOUSE_LEFT_Y - start_y + delta_y * j) / 20.0)*(-1);
				GLB::MOUSE_LEFT = false;
				civiliz_Form[j].open_close();

				if (i > 0) {
					civiliz_Form[j].select_option(i);
				}
			}
			if (pickingList[clickId] == "ColForm_" + std::to_string(j)) {
				GLB::MOUSE_LEFT = false;
				players_color[j] ++;
				if (players_color[j] == GLB::COLORS.size()) { players_color[j] = 0; }
				bool c = false;
				while (!c) {
					int s = 0;
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


Menu::~Menu()
{
}
