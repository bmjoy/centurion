#include "debug_ui.h"



DebugUI::DebugUI()
{
	staticTextList = {
		"frames/s: ",
		"ms/frame: ",
		"y-window: ",
		"x-window: ",
		"y-map: ",
		"x-map: ",
		"selected units: "
	};
	dynamicTextList = { "0" };

	startX = 14.f;
	startY = GAME::UI_BOTTOM_HEIGHT + 12.f;
	deltaY = 15.f;
}

void DebugUI::create() {

	dynamicText = gui::SimpleText("dynamic");

	for (int i = 0; i < staticTextList.size(); i++) {
		gui::SimpleText tempText = gui::SimpleText("static");
		tempText.create_static(staticTextList[i], "tahoma_6", startX, startY + deltaY * i, "left", "normal", glm::vec4(255.f));
		staticText.push_back(tempText);
	}

	back = gui::Rectangle();
	back.set_color(glm::vec4(0.f, 0.f, 0.f, 0.5f));
	back.create("filled", 10.f, GAME::UI_BOTTOM_HEIGHT + 10.f, 220.f, 200.f, "bottom-left");

}

void DebugUI::render(int fps, int mpfs, int selUnits) {

	back.render();

	dynamicTextList[0] = std::to_string(fps);
	dynamicTextList[1] = std::to_string(mpfs);
	dynamicTextList[2] = std::to_string(GLB::MOUSE_Y);
	dynamicTextList[3] = std::to_string(GLB::MOUSE_X);
	dynamicTextList[4] = std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).y);
	dynamicTextList[5] = std::to_string((int)getZoomedCoords((float)GLB::MOUSE_X, (float)GLB::MOUSE_Y).x);
	dynamicTextList[6] = std::to_string(selUnits);

	!GLB::GAME ? n = 2 : n = 6;

	for (int i = 0; i < n; i++) {
		staticText[i].render_static();
		dynamicText.set_position(glm::vec2(startX + 100.f, startY + deltaY * i));
		dynamicText.set_text(dynamicTextList[i]);
		dynamicText.render_dynamic("tahoma_6", glm::vec4(255.f), "left", "normal");
	}
}

DebugUI::~DebugUI()
{
}
