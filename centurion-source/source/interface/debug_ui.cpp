#include <interface>
#include <engine>
#include <game>

using namespace std;
using namespace glm;
using namespace glb;
using namespace engine;

namespace debug {

	DebugUI *DEBUG_UI() { return &mydebugui; }

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
		debuguiIsActive = true;
	}

	void DebugUI::create() {
		startX = 14.f;
		startY = getParam("ui-bottom-height") + 12.f;
		deltaY = 15.f;

		dynamicText = gui::SimpleText("dynamic");

		debugText[0] = gui::SimpleText("static");
		debugText[0].create_static("Centurion - DEBUG MODE", "couriernew_15px", startX, startY + 177, "left", "normal", vec4(255,255,0,255));
		debugText[1] = gui::SimpleText("static");
		debugText[1].create_static("Currentlang:  " + language, "couriernew_15px", startX, startY + 157, "left", "normal", vec4(0, 255, 0, 255));

		for (int i = 0; i < staticTextList.size(); i++) {
			gui::SimpleText tempText = gui::SimpleText("static");
			tempText.create_static(staticTextList[i], "couriernew_15px", startX, startY + deltaY * i, "left", "normal", vec4(255.f));
			staticText.push_back(tempText);
		}

		back = gui::Rectangle();
		back.create("border-filled", 10.f, getParam("ui-bottom-height") + 10.f, 250.f, 200.f, "bottom-left", 0);
	
		currentLan = language;
	}

	void DebugUI::render(int fps, int mpfs, int selUnits) {

		if (debuguiIsActive && getBoolean("debug")) {

			if (currentLan != language) {
				debugText[1].create_static("Currentlang:  " + language, "couriernew_15px", startX, startY + 157, "left", "normal", vec4(0, 255, 0, 255));
				currentLan = language;
			}

			back.render(vec4(0.f, 0.f, 0.f, 0.5f));

			debugText[0].render_static();
			debugText[1].render_static();

			dynamicTextList[0] = to_string(fps);
			dynamicTextList[1] = to_string(mpfs);
			dynamicTextList[2] = to_string((int)getParam("mouse-y-position"));
			dynamicTextList[3] = to_string((int)getParam("mouse-x-position"));
			dynamicTextList[4] = to_string((int)getZoomedCoords(getParam("mouse-x-position"), getParam("mouse-y-position")).y);
			dynamicTextList[5] = to_string((int)getZoomedCoords(getParam("mouse-x-position"), getParam("mouse-y-position")).x);
			dynamicTextList[6] = to_string(selUnits);

			!(ENGINE()->getEnvironment() == "game") ? n = 2 : n = (int)dynamicTextList.size();

			for (int i = 0; i < n; i++) {
				staticText[i].render_static();
				dynamicText.render_dynamic(dynamicTextList[i], "couriernew_15px", startX + 150.f, startY + deltaY * i, vec4(255.f), "left", "normal");
			}
		}
	}

	DebugUI::~DebugUI()
	{
	}
};