#include "game_ui.h"



UIGame::UIGame()
{
	time.seconds = 0; time.seconds_str = "00";
	time.minutes = 0; time.minutes_str = "00";
	time.hours = 0; time.hours_str = "00";
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

	time.lastTime = glfwGetTime();
	time.text = gui::SimpleText("dynamic", true);
}

void UIGame::render() {

	gameMenu.render(true);

	picking();

	calculateTime();
	gameMenu.render(false);
		
	time.text.set_position(glm::vec2(GLB::WINDOW_WIDTH - 100.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 30.0f));
	time.text.set_text(time.hours_str + ":" + time.minutes_str + ":" + time.seconds_str);
	time.text.render_dynamic("tahoma_8", glm::vec4(255.f, 255.f, 255.f, 255.f), "left", "normal");

	// Temporary bars:
	top_bar.render();
	bottom_bar.render();	
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

void UIGame::calculateTime() {
	if (glfwGetTime() - time.lastTime >= 1.0f) {
		time.seconds++;
		time.lastTime += 1.0f;
	}
	if (time.seconds == 60) {
		time.seconds = 0;
		time.minutes++;
	}
	if (time.minutes == 60) {
		time.minutes = 0;
		time.hours++;
	}
	if (time.seconds < 10) { time.seconds_str = "0" + std::to_string(time.seconds); }
	else { time.seconds_str = std::to_string(time.seconds); }
	if (time.minutes < 10) { time.minutes_str = "0" + std::to_string(time.minutes); }
	else { time.minutes_str = std::to_string(time.minutes); }
	if (time.hours < 10) { time.hours_str = "0" + std::to_string(time.hours); }
	else { time.hours_str = std::to_string(time.hours); }
}

UIGame::~UIGame()
{
}
