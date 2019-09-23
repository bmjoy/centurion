#include "ui_game.h"



UIGame::UIGame()
{
	
}

void UIGame::create(int *pickingId) {

	objectId = pickingId;

	ui_top = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	ui_top.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	ui_top.create(GLB::WINDOW_WIDTH, GAME::UI_TOP_HEIGHT, "bottom-left");
	
	ui_bottom = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	ui_bottom.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	ui_bottom.create(GLB::WINDOW_WIDTH, GAME::UI_BOTTOM_HEIGHT, "bottom-left");
	
	gameMenu = GameMenu();
	gameMenu.create(objectId, &pickingList);
}

void UIGame::render(bool pick) {
	
	if (pick) {
		gameMenu.render(true);

		picking();
	}

	else {
		ui_bottom.apply_projection_matrix(GLB::MENU_PROJECTION);
		ui_bottom.apply_view_matrix();
		ui_bottom.render(0.0f, 0.0f);

		ui_top.apply_projection_matrix(GLB::MENU_PROJECTION);
		ui_bottom.apply_view_matrix();
		ui_top.render(0.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT);

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
