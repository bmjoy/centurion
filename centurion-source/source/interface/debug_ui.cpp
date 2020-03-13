#include <interface>
#include <engine.h>
#include <game/strategy.h>
#include <picking.h>

#include <global>

using namespace std;
using namespace glm;
using namespace glb;



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
			"picking obj: ",
			"picking ui: ",
		};
		dynamicTextList = { "0" };
		debuguiIsActive = true;
	}

	void DebugUI::create() {
		startX = 14.f;
		startY = Engine::myWindow::BottomBarHeight + 12.f;
		deltaY = 15.f;

		dynamicText = gui::SimpleText("dynamic");

		debugText[0] = gui::SimpleText("static");
		debugText[0].create_static("Centurion - DEBUG MODE", "couriernew_15px", startX, startY + 177, "left", "normal", vec4(255,255,0,255));
		debugText[1] = gui::SimpleText("static");
		debugText[1].create_static("Currentlang:  " + glb::Settings::Language, "couriernew_15px", startX, startY + 157, "left", "normal", vec4(0, 255, 0, 255));

		for (int i = 0; i < staticTextList.size(); i++) {
			gui::SimpleText tempText = gui::SimpleText("static");
			tempText.create_static(staticTextList[i], "couriernew_15px", startX, startY + deltaY * i, "left", "normal", vec4(255.f));
			staticText.push_back(tempText);
		}

		back = gui::Rectangle();
		back.create("border-filled", 10.f, Engine::myWindow::BottomBarHeight + 10.f, 250.f, 200.f, "bottom-left", 0);
	
		currentLan = Settings::Language;
	}

	void DebugUI::render(int fps, int mpfs, int selUnits) {

		if (debuguiIsActive && Settings::DebugIsActive) {

			if (currentLan != glb::Settings::Language) {
				debugText[1].create_static("Currentlang:  " + glb::Settings::Language, "couriernew_15px", startX, startY + 157, "left", "normal", vec4(0, 255, 0, 255));
				currentLan = glb::Settings::Language;
			}

			back.render(vec4(0.f, 0.f, 0.f, 0.5f));

			debugText[0].render_static();
			debugText[1].render_static();

			dynamicTextList[0] = to_string(fps);
			dynamicTextList[1] = to_string(mpfs);
			dynamicTextList[2] = to_string((int)Engine::Mouse::GetYPosition());
			dynamicTextList[3] = to_string((int)Engine::Mouse::GetXPosition());
			dynamicTextList[4] = to_string((int)Engine::Camera::GetZoomedCoords(Engine::Mouse::GetXPosition(), Engine::Mouse::GetYPosition()).y);
			dynamicTextList[5] = to_string((int)Engine::Camera::GetZoomedCoords(Engine::Mouse::GetXPosition(), Engine::Mouse::GetYPosition()).x);
			dynamicTextList[6] = to_string(selUnits);
			dynamicTextList[7] = to_string(PickingObject::getLastPickingID());
			dynamicTextList[8] = to_string(PickingUI::getLastID());

			//!(Engine::getEnvironment() == "game") ? n = 2 : n = (int)dynamicTextList.size();
			n = (int)dynamicTextList.size();
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