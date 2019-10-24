#include <interface>
#include <picking>
#include <engine>
#include <editor>
#include <surface>

using namespace glb;
using namespace engine;

namespace editor {

	TerrainBrushWindow::TerrainBrushWindow(){
		terrainMap["EDITORBRUSH_grass1"] = 1;
		terrainMap["EDITORBRUSH_road1"] = 2;
	}

	void TerrainBrushWindow::create() {

		startX = 0.f; startY = getParam("window-height")/2.f + 150.f / 2.f;

		back = gui::Rectangle();
		back.create("filled", startX, startY, 190.f, 150.f, "top-left", getPickingID());
		increasePickingID();

		circle = gui::Circle();
		circle.create("border", getParam("mouse-x-position"), getParam("mouse-y-position"), 150.f, 100.f, 5.f, "center");

		vector<string> terrainList;
		for (map<string, int>::iterator i = terrainMap.begin(); i != terrainMap.end(); i++)
			terrainList.push_back(i->first);
		form1 = gui::FormInput(true);
		form1.create(startX + 20.f, startY - 80.f, 150.f, 20.f, terrainList, getPickingID());
		addValueToPickingListUI(getPickingID(), "TerrainBrush_form1");
		increasePickingID();

		TerrainBrushWindowIsOpen = false;
	}
	void TerrainBrushWindow::render(bool pick) {

		if (getBoolean("mouse-right")) TerrainBrushIsActive = false;
		if (getBoolean("mouse-left") && getParam("mouse-y-position") >= getParam("window-height") - 30.f) TerrainBrushIsActive = false;

		if (TerrainBrushWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				back.render(vec4(), true);
				form1.render(true, vec4());
				picking();
			}
			if (!pick) {

				if (TerrainBrushIsActive){
					circle.render(vec4(255), getParam("mouse-x-position"), getParam("mouse-y-position"));
					if (getBoolean("mouse-left-pressed") && leftClickID_UI == 0 && getParam("mouse-y-position") < getParam("window-height") - 30.f) {
						changeTerrain(terrainMap[form1.selectedText]);
					}
				}

				back.render(vec4(60.f, 68.f, 104.f, 255.f), false);
				form1.render(false, vec4(60.f, 68.f, 104.f, 255.f));
			}
		}
	}

	TerrainBrushWindow::~TerrainBrushWindow(){}
};
