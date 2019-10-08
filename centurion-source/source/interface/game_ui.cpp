#include "game_ui.h"

#include <picking>

using namespace glb;

UIGame::UIGame()
{
	time.seconds = 0; time.seconds_str = "00";
	time.minutes = 0; time.minutes_str = "00";
	time.hours = 0; time.hours_str = "00";
	time.x = getParam("window-width") - 100.0f;
	time.y = getParam("window-height") - getParam("ui-top-height") - 30.0f;

}

void UIGame::create() {

	gameMenu = GameMenu();
	gameMenu.create();

	top_bar = gui::Rectangle();
	top_bar.create("filled", 0.0f, getParam("window-height") - getParam("ui-top-height"), getParam("window-width"), getParam("ui-top-height"), "bottom-left", 0);

	bottom_bar = gui::Rectangle();
	bottom_bar.create("filled", 0.0f, 0.0f, getParam("window-width"), getParam("ui-bottom-height"), "bottom-left", 0);

	time.lastTime = glfwGetTime();
	time.text = gui::SimpleText("dynamic", true);

	minimapRectangle = gui::Rectangle();
	minimapRectangle.create("border", 0, 0, 0, 0, "bottom-left", 0);
}

void UIGame::render() {

	if (GLB::MOUSE_LEFT){
		gameMenu.render(true);
		picking();
	}

	calculateTime();
	gameMenu.render(false);
		
	std::string txt = time.hours_str + ":" + time.minutes_str + ":" + time.seconds_str;
	time.text.render_dynamic(txt, "tahoma_8", time.x, time.y, glm::vec4(255.f, 255.f, 255.f, 255.f), "left", "normal");

	// minimap rectangle:
	if (GAME::MINIMAP_IS_ACTIVE) {
		float x = getParam("camera-x-position") / GAME::MAP_WIDTH * getParam("window-width");
		float y = getParam("camera-y-position") / GAME::MAP_HEIGHT * (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")) + getParam("ui-bottom-height");
		float w = getParam("window-width-zoomed") * getParam("window-width") / GAME::MAP_WIDTH;
		float h = getParam("window-height-zoomed") * (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")) / GAME::MAP_HEIGHT;
		x = std::max(x, 1.f);
		y = std::max(y, getParam("ui-bottom-height") + 1.f);
		y = std::min(y, getParam("window-height") - getParam("ui-top-height") - h);
		minimapRectangle.render(glm::vec4(255.f), false, x, y, w, h);
	}
	// Temporary bars:
	top_bar.render(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
	bottom_bar.render(glm::vec4(255.0f, 0.0f, 0.0f, 1.0f));
}

void UIGame::picking() {
	int clickId = get_id();
	string clickName = getPickedObjectName(clickId);
	if (clickName == "menuButtonClose") {
		GAME::MENU_IS_ACTIVE = false;
		GLB::MOUSE_LEFT = false;
	}
	if (clickName == "menuButtonQuit") {
		GAME::MENU_IS_ACTIVE = false;
		GLB::RESET = true;
		GLB::MOUSE_LEFT = false;
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
