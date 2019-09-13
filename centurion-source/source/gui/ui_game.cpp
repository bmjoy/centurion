#include "ui_game.h"



UIGame::UIGame()
{
}

void UIGame::create() {
	ui_top = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	ui_top.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	ui_top.create(GLB::WINDOW_WIDTH, GAME::UI_TOP_HEIGHT, "bottom-left");
	
	ui_bottom = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	ui_bottom.set_color(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	ui_bottom.create(GLB::WINDOW_WIDTH, GAME::UI_BOTTOM_HEIGHT, "bottom-left");
	
}

void UIGame::render() {
	
	ui_bottom.apply_projection_matrix(GLB::MENU_PROJECTION);
	ui_bottom.apply_view_matrix();
	ui_bottom.render(0.0f, 0.0f);
	
	ui_top.apply_projection_matrix(GLB::MENU_PROJECTION);
	ui_bottom.apply_view_matrix();
	ui_top.render(0.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT);
}

UIGame::~UIGame()
{
}
