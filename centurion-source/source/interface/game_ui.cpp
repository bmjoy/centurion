#include "game_ui.h"



UIGame::UIGame()
{
	
}

void UIGame::create(int *pickingId) {

	objectId = pickingId;

	gameMenu = GameMenu();
	gameMenu.create(objectId, &pickingList);

	top_bar = gui::Rectangle();
	top_bar.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	top_bar.create("filled", 0.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT, GLB::WINDOW_WIDTH, GAME::UI_TOP_HEIGHT, "bottom-left");

	bottom_bar = gui::Rectangle();
	bottom_bar.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	bottom_bar.create("filled", 0.0f, 0.0f, GLB::WINDOW_WIDTH, GAME::UI_BOTTOM_HEIGHT, "bottom-left");
}

void UIGame::render(bool pick) {
	
	if (pick) {
		gameMenu.render(true);

		picking();
	}

	else {

		// Temporary bars:
		top_bar.render();
		bottom_bar.render();
		gameMenu.render(false);
	}
	
}

void UIGame::picking() {
	if (GLB::MOUSE_LEFT) {

		clickId = get_id();
		if (pickingList[clickId] == "menuButtonClose") {
			GAME::MENU_IS_ACTIVE = false;
			GLB::MOUSE_LEFT = false;
		}
		if (pickingList[clickId] == "menuButtonQuit") {
			GAME::MENU_IS_ACTIVE = false;
			GLB::GAME_CLEAR = true;
			GLB::MOUSE_LEFT = false;
		}

	}
}

UIGame::~UIGame()
{
}
