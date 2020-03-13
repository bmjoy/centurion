#include <interface>
#include <picking.h>
#include <game/editor.h>
#include <surface>
#include <game/strategy.h>
#include <engine.h>

#include <primitives.h>
#include <terrain.h>

using namespace glb;


namespace editor {

	TerrainBrushWindow::TerrainBrushWindow(){
	}

	void TerrainBrushWindow::create() {

		startX = 0.f; startY = Engine::myWindow::Height/2.f + 150.f / 2.f;

		back = gui::Rectangle();
		back.create("filled", startX, startY, 190.f, 150.f, "top-left", PickingUI::obtainPickingID());

		circle = gui::Circle();
		circle.create("border", Engine::Mouse::GetXPosition(), Engine::Mouse::GetYPosition(), 150.f, 100.f, 5.f, "center");

		vector<string> terrainList = MapTerrain()->getTerrainList();
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
			form.create(startX + 20.f, startY - 20.f - 30 * i, 150.f, 20.f, terTypesMap[terrainTypes[i]], PickingUI::obtainPickingID());
			PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "TerrainBrush_form_"+i);
			forms.push_back(form);
		}

		selBrush = "N/A";
		TerrainBrushWindowIsOpen = false;
	}
	void TerrainBrushWindow::render(bool pick) {

		if (Engine::Mouse::RightClick) TerrainBrushIsActive = false;
		if (Engine::Mouse::LeftClick && Engine::Mouse::GetYPosition() >= Engine::myWindow::Height - 30.f) TerrainBrushIsActive = false;

		if (TerrainBrushWindowIsOpen) {
			if (pick && Engine::Mouse::LeftClick) {
				back.render(vec4(), true);
				for (int i = (int)forms.size()-1; i >= 0; i--) { forms[i].render(true, vec4()); }
				picking();
			}
			if (!pick) {
				if (Game::Minimap::IsActive() == false && TerrainBrushIsActive){
					circle.render(vec4(255), Engine::Mouse::GetXPosition(), Engine::Mouse::GetYPosition());
					if (Engine::Mouse::LeftHold && Picking::leftClickID_UI == 0 && Engine::Mouse::GetYPosition() < Engine::myWindow::Height - 30.f) {
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
