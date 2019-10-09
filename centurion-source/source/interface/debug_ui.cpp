#include "debug_ui.h"
#include <engine>
#include <game>

using namespace glb;
using namespace engine;

DebugUI::DebugUI()
{
	staticTextList = {
		"frames/s: ",
		"ms/frame: ",
		"y-window: ",
		"x-window: ",
		"y-map: ",
		"x-map: ",
		"selected units: ",
	};
	dynamicTextList = { "0" };

	startX = 14.f;
	startY = getParam("ui-bottom-height") + 12.f;
	deltaY = 15.f;
}

void DebugUI::create() {

	dynamicText = gui::SimpleText("dynamic");

	debugText = gui::SimpleText("static");
	debugText.create_static("Centurion - DEBUG MODE", "inconsolata_8", startX, startY + 177, "left", "normal", glm::vec4(255,255,0,255));

	for (int i = 0; i < staticTextList.size(); i++) {
		gui::SimpleText tempText = gui::SimpleText("static");
		tempText.create_static(staticTextList[i], "inconsolata_8", startX, startY + deltaY * i, "left", "normal", glm::vec4(255.f));
		staticText.push_back(tempText);
	}

	back = gui::Rectangle();
	back.create("border-filled", 10.f, getParam("ui-bottom-height") + 10.f, 186, 200.f, "bottom-left", 0);
	
	
}

void DebugUI::render(int fps, int mpfs, int selUnits) {
	back.render(glm::vec4(0.f, 0.f, 0.f, 0.5f));

	debugText.render_static();

	dynamicTextList[0] = std::to_string(fps);
	dynamicTextList[1] = std::to_string(mpfs);
	dynamicTextList[2] = std::to_string((int)getParam("mouse-y-position"));
	dynamicTextList[3] = std::to_string((int)getParam("mouse-x-position"));
	dynamicTextList[4] = std::to_string((int)getZoomedCoords(getParam("mouse-x-position"), getParam("mouse-y-position")).y);
	dynamicTextList[5] = std::to_string((int)getZoomedCoords(getParam("mouse-x-position"), getParam("mouse-y-position")).x);
	dynamicTextList[6] = std::to_string(selUnits);

	!(ENGINE()->getEnvironment() == "game") ? n = 2 : n = (int)dynamicTextList.size();

	for (int i = 0; i < n; i++) {
		staticText[i].render_static();
		dynamicText.render_dynamic(dynamicTextList[i], "inconsolata_8", startX + 135.f, startY + deltaY * i, glm::vec4(255.f), "left", "normal");
	}
}

DebugUI::~DebugUI()
{
}
