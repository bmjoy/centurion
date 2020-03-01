#include <interface>
#include <picking>
#include <engine>
#include <editor>
#include <surface>
#include <game>

using namespace glb;
using namespace engine;

namespace editor {

	TerrainBrushWindow::TerrainBrushWindow(){
	}

	void TerrainBrushWindow::create() {

		startX = 0.f; startY = Settings::WindowHeight()/2.f + 150.f / 2.f;

		back = gui::Rectangle();
		back.create("filled", startX, startY, 190.f, 150.f, "top-left", getPickingID());
		increasePickingID();

		circle = gui::Circle();
		circle.create("border", getParam("mouse-x-position"), getParam("mouse-y-position"), 150.f, 100.f, 5.f, "center");

		vector<string> terrainList = obj::MapTerrain()->getTerrainList();
		vector<string> terrainTypes;
		map<string, vector<string>> terTypesMap;

		for (int i = 0; i < terrainList.size(); i++) {
			string temp = terrainList[i];
			terrainTypes.push_back(temp.erase(temp.find_first_of('_'),temp.length()));
			terrainList[i] = "EDITORBRUSH_" + terrainList[i];
			terTypesMap[terrainTypes[i]].push_back(terrainList[i]);
			//terrainMap[terrainList[i]] = i + 1;
		}
		sort(terrainTypes.begin(), terrainTypes.end());
		terrainTypes.erase(unique(terrainTypes.begin(), terrainTypes.end()), terrainTypes.end());

		for (int i = 0; i < terrainTypes.size(); i++){
			gui::FormInput form = gui::FormInput(true);
			form.create(startX + 20.f, startY - 20.f - 30 * i, 150.f, 20.f, terTypesMap[terrainTypes[i]], getPickingID());
			addValueToPickingListUI(getPickingID(), "TerrainBrush_form_"+i);
			increasePickingID();
			forms.push_back(form);
		}

		selBrush = "N/A";
		TerrainBrushWindowIsOpen = false;
	}
	void TerrainBrushWindow::render(bool pick) {

		if (getBoolean("mouse-right")) TerrainBrushIsActive = false;
		if (getBoolean("mouse-left") && getParam("mouse-y-position") >= Settings::WindowHeight() - 30.f) TerrainBrushIsActive = false;

		if (TerrainBrushWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				back.render(vec4(), true);
				for (int i = (int)forms.size()-1; i >= 0; i--) { forms[i].render(true, vec4()); }
				picking();
			}
			if (!pick) {
				if (!game::gameMinimapStatus && TerrainBrushIsActive){
					circle.render(vec4(255), getParam("mouse-x-position"), getParam("mouse-y-position"));
					if (getBoolean("mouse-left-pressed") && leftClickID_UI == 0 && getParam("mouse-y-position") < Settings::WindowHeight() - 30.f) {						
						if (mapgen::terrainsMap.count(selBrush) > 0) changeTerrain(mapgen::terrainsMap[selBrush].id);
					}
				}

				back.render(vec4(60.f, 68.f, 104.f, 255.f), false);
				for (int i = (int)forms.size() - 1; i >= 0; i--) { forms[i].render(false, vec4(60.f, 68.f, 104.f, 255.f)); }
			}
		}
	}

	TerrainBrushWindow::~TerrainBrushWindow(){}
};
