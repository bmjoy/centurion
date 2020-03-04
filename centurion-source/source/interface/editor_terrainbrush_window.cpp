#include <interface>
#include <picking>
#include <game/editor.h>
#include <surface>
#include <game/strategy.h>
#include <engine/mouse.h>
#include <engine/window.h>

using namespace glb;
using namespace engine;

namespace editor {

	TerrainBrushWindow::TerrainBrushWindow(){
	}

	void TerrainBrushWindow::create() {

		startX = 0.f; startY = myWindow::Height/2.f + 150.f / 2.f;

		back = gui::Rectangle();
		back.create("filled", startX, startY, 190.f, 150.f, "top-left", getPickingID_UI());
		increasePickingID_UI();

		circle = gui::Circle();
		circle.create("border", engine::Mouse::GetXPosition(), engine::Mouse::GetYPosition(), 150.f, 100.f, 5.f, "center");

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
			form.create(startX + 20.f, startY - 20.f - 30 * i, 150.f, 20.f, terTypesMap[terrainTypes[i]], getPickingID_UI());
			addValueToPickingListUI(getPickingID_UI(), "TerrainBrush_form_"+i);
			increasePickingID_UI();
			forms.push_back(form);
		}

		selBrush = "N/A";
		TerrainBrushWindowIsOpen = false;
	}
	void TerrainBrushWindow::render(bool pick) {

		if (engine::Mouse::RightClick) TerrainBrushIsActive = false;
		if (engine::Mouse::LeftClick && engine::Mouse::GetYPosition() >= myWindow::Height - 30.f) TerrainBrushIsActive = false;

		if (TerrainBrushWindowIsOpen) {
			if (pick && engine::Mouse::LeftClick) {
				back.render(vec4(), true);
				for (int i = (int)forms.size()-1; i >= 0; i--) { forms[i].render(true, vec4()); }
				picking();
			}
			if (!pick) {
				if (!gameMinimapStatus && TerrainBrushIsActive){
					circle.render(vec4(255), engine::Mouse::GetXPosition(), engine::Mouse::GetYPosition());
					if (Mouse::LeftHold && leftClickID_UI == 0 && engine::Mouse::GetYPosition() < myWindow::Height - 30.f) {
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
