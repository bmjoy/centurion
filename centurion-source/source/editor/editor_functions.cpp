#include <editor>
#include <interface>
#include <global>
#include <picking>
#include <player>
#include <game>
#include <engine>
#include <surface>
#include <settings.h>

using namespace building;
using namespace unit;
using namespace decoration;
using namespace glb;

namespace editor {

	/* extern variables definitions */

    bool menuIsOpened = false;
	bool IsWindowOpened = false;
	bool OpenMapWindowIsOpen = false;
	bool OpenMapWindowUpdate = false;
	vector<string> availableScenarios;
	bool NewMapWindowIsOpen = true;
	bool NewMapResetText = false;
	string currentMapName;
	bool AddObjectWindowIsOpen = false;
	bool AddObjectWindowUpdateForm1and2 = false;
	bool AddObjectWindowUpdateForm2 = false;
	bool PropertiesWindowIsOpen = false;
	bool PropertiesWindowResetText = false;
	bool TerrainBrushWindowIsOpen = false;
	bool TerrainBrushIsActive = false;
	bool QuestionWindowIsOpen = false;
	bool addingObject = false;
	bool movingObject = false;
	gui::SimpleText textInfo = gui::SimpleText("dynamic");

	string EditorObjectStringListForm0[NumberOfObjects] = { "" };
	string EditorObjectStringListForm1[NumberOfObjects] = { "" };
	string EditorObjectStringListForm2[NumberOfObjects] = { "" };

	vector<string> EditorAddObjectBuildingOptions;
	vector<string> EditorAddObjectUnitOptions;
	vector<string> EditorAddObjectDecorationOptions;

	Unit unitTemp;
	Building buildingTemp;
	Decoration decorTemp;

	Editor *EDITOR() { return &myeditor; }
	QuestionWindow *Q_WINDOW() { return &myquestionwindow; }

	void Editor::handleKeyboardControls() {
		//CTRL Hotkeys
		if (!IsWindowOpened) {
			if (KeyCode[GLFW_KEY_LEFT_CONTROL] || KeyCode[GLFW_KEY_RIGHT_CONTROL]) {
				if (KeyCode[GLFW_KEY_N]) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
				if (KeyCode[GLFW_KEY_O]) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
				if (KeyCode[GLFW_KEY_S]) { saveCurrentScenario(currentMapName); }
				if (KeyCode[GLFW_KEY_A]) { TerrainBrushIsActive = false; TerrainBrushWindowIsOpen = false; AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
				if (KeyCode[GLFW_KEY_T]) { AddObjectWindowIsOpen = false; TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen; }
			}
			if (KeyCode[GLFW_KEY_DELETE]) {
				if (game::buildings.count(leftClickID) > 0) {
					if (game::buildings[leftClickID].isSelected()) {
						if (game::buildings[leftClickID].is_independent()) {
							if (game::buildings[leftClickID].buildingsInSettlementCount() > 0) {
								game::buildings[leftClickID].setWaitingToBeErased(true);
								Q_WINDOW()->setQuestion("QUESTION_deleteAll");
							}
							else {
								cout << "[DEBUG] Settlement " << game::buildings[leftClickID].get_name() << " deleted!\n";
								game::buildings[leftClickID].clear_pass();
								game::buildings.erase(leftClickID);
							}
						}
						else {
							cout << "[DEBUG] Building " << game::buildings[leftClickID].get_name() << " deleted!\n";
							game::buildings[leftClickID].clear_pass();
							game::buildings.erase(leftClickID);
						}
					}
				}
			}
			if (KeyCode[GLFW_KEY_SPACE] || getBoolean("mouse-middle")) {
				game::gameMinimapStatus = !game::gameMinimapStatus;
				game::gameMinimapStatus ? std::cout << "[DEBUG] Minimap ON!\n" : std::cout << "[DEBUG] Minimap OFF!\n";
			}
			if (KeyCode[GLFW_KEY_Z]) {
				setBoolean("wireframe", !getBoolean("wireframe"));
				getBoolean("wireframe") ? std::cout << "[DEBUG] Wireframe ON!\n" : std::cout << "[DEBUG] Wireframe OFF! \n";
			}
			// Grid
			/*if (KeyCode[GLFW_KEY_G]) {
				surface->updateGrid();
				game::gameGridStatus = !game::gameGridStatus;
				game::gameGridStatus ? std::cout << "[DEBUG] Grid ON!\n" : std::cout << "[DEBUG] Grid OFF!\n";
			}*/
		}
		if (KeyCode[GLFW_KEY_ESCAPE]) {
			if (areWindowsClosed()) {
				clearEditorVariables();
				engine::ENGINE()->Reset();
			}
			else {
				clearEditorVariables();
				EDITOR_UI()->close_menu();
			}
		}
	}

	/* tools */
	void prepareObject(string type, string classname) {
		if (type == "buildings") {
			buildingTemp.set_class(classname);
			buildingTemp.set_id(0);
			buildingTemp.set_player(0);
			buildingTemp.set_position(vec3(0));
			buildingTemp.prepare();
		}
		if (type == "decorations") {
			decorTemp.set_class(classname);
			decorTemp.set_id(0);
			decorTemp.set_player(0);
			decorTemp.set_position(vec3(0));
			decorTemp.prepare();
		}
	}

	void insertingObject(string type, string classname) {
		float x = round(getParam("mouse-x-position") * getParam("window-width-zoomed") / settings.GetWindowWidth() + getParam("camera-x-position"));
		float y = round(getParam("mouse-y-position") * getParam("window-height-zoomed") / settings.GetWindowHeight() + getParam("camera-y-position"));
		if (type == "buildings") {
			buildingTemp.set_position(vec3(x, y, 0.f));
			buildingTemp.set_status(false);
			buildingTemp.render(false, 0, !buildingTemp.is_placeable());
			
			//Player will be able to see info about placing status
			if (!buildingTemp.is_independent()) {
				string s = "";
				if (!buildingTemp.is_near_to_independent(&s))
					textInfo.render_dynamic(getTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
				else
					if (!buildingTemp.is_placeable())
						textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
			}
			else {
				if (!buildingTemp.is_placeable())
					textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
				else
					textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
			}
		}
		if (type == "decorations") {
			decorTemp.set_position(vec3(x, y, 0.f));
			decorTemp.render(!decorTemp.is_placeable());
		}
	}

	void addObject(string type) {
		if (type == "buildings") {
			if (buildingTemp.is_placeable()){
				int ID = getPickingID(); increasePickingID();
				buildingTemp.set_id(ID);
				buildingTemp.create();
				game::buildings[ID] = buildingTemp;
				if (game::buildings[ID].is_independent()) {
					game::buildings[ID].set_settlement_name("SETTL_" + game::buildings[ID].get_name());
					game::independent_buildings[ID] = &game::buildings[ID];
				}
				setBoolean("mouse-left", false);
				addingObject = false;
			}
		}
		if (type == "decorations") {
			if (decorTemp.is_placeable()) {
				int ID = getPickingID(); increasePickingID();
				decorTemp.set_id(ID);
				decorTemp.create();
				game::decorations[ID] = decorTemp;
				setBoolean("mouse-left", false);
				addingObject = false;
			}
		}
		game::MINIMAP()->setStatus(false);
	}

	void changeTerrain(int terrainType) {
		float x1 = (getParam("mouse-x-position") * getParam("window-width-zoomed") / settings.GetWindowWidth() + getParam("camera-x-position"));
		float y1 = (getParam("mouse-y-position") * getParam("window-height-zoomed") / settings.GetWindowHeight() + getParam("camera-y-position"));
		float type = float(terrainType);
		
		int x = int(round(x1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int y = int(round(y1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		// + mapgen::grid_size * 2; because the map has "borders"

		int j = mapgen::getVertexPos(x, y);
	
		if (mapgen::MapTextures()[j] != type){
			mapgen::MapTextures()[j] = type;
			obj::MapTerrain()->updateTextureBuffer();
			game::MINIMAP()->setStatus(false);
		}
	}

	void clearEditorVariables() {
		IsWindowOpened = false;
		QuestionWindowIsOpen = false;
		NewMapWindowIsOpen = false;
		NewMapResetText = false;
		PropertiesWindowIsOpen = false;
		PropertiesWindowResetText = false;
		OpenMapWindowIsOpen = false;
		OpenMapWindowUpdate = false;
		AddObjectWindowIsOpen = false;
		AddObjectWindowUpdateForm1and2 = false;
		AddObjectWindowUpdateForm2 = false;
		TerrainBrushWindowIsOpen = false;
		TerrainBrushIsActive = false;
		menuIsOpened = false;
		addingObject = false;
	}

	bool areWindowsClosed() {
		if(IsWindowOpened || menuIsOpened)
			return false;
		return true;
	}

	void moveObjects() {
		if (getBoolean("mouse-left-pressed")) {
			// buildings
			if (game::buildings.count(leftClickID) > 0) {
				movingObjectRestore = false;
				if (!movingObject) {
					movingObjectXPos = game::buildings[leftClickID].get_position().x;
					movingObjectYPos = game::buildings[leftClickID].get_position().y;
					movingObjectStartXMouse = getParam("mouse-x-position") * getParam("window-width-zoomed") / settings.GetWindowWidth() + getParam("camera-x-position");
					movingObjectStartYMouse = getParam("mouse-y-position") * getParam("window-height-zoomed") / settings.GetWindowHeight() + getParam("camera-y-position");
				}
				float x1 = (getParam("mouse-x-position") * getParam("window-width-zoomed") / settings.GetWindowWidth() + getParam("camera-x-position"));
				float y1 = (getParam("mouse-y-position") * getParam("window-height-zoomed") / settings.GetWindowHeight() + getParam("camera-y-position"));
				float dx = x1 - movingObjectStartXMouse;
				float dy = y1 - movingObjectStartYMouse;

				if (!game::buildings[leftClickID].is_independent()){
					if (!movingObject) game::buildings[leftClickID].clear_pass();
					game::buildings[leftClickID].set_position(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!game::buildings[leftClickID].is_placeable()) {
						string s = "";
						if (!game::buildings[leftClickID].is_near_to_independent(&s)) {
							textInfo.render_dynamic(getTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
						}
						else
							textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
						game::buildings[leftClickID].set_placeable(false);
						movingObjectRestore = true;
					}
					else {
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
						game::buildings[leftClickID].set_placeable(true);
					}
				}
				else {
					if (!movingObject) game::buildings[leftClickID].clear_pass();
					game::buildings[leftClickID].set_position(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!game::buildings[leftClickID].is_placeable()) {
						game::buildings[leftClickID].set_placeable(false);
						movingObjectRestore = true;
					}
					else {
						game::buildings[leftClickID].set_placeable(true);
					}
					if (!game::buildings[leftClickID].is_placeable())
						textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, settings.GetWindowHeight() - 50, vec4(255.f), "left", "center");
				}
				movingObject = true;
				game::MINIMAP()->setStatus(false);
			}
		}
		else {
			// buildings
			if (game::buildings.count(leftClickID) > 0) {
				if (movingObjectRestore) {
					game::buildings[leftClickID].set_position(vec3(movingObjectXPos, movingObjectYPos, 0.f));
					game::buildings[leftClickID].clear_pass();
					game::buildings[leftClickID].set_placeable(true);
					game::buildings[leftClickID].update_pass();
				}
				else {
					game::buildings[leftClickID].set_placeable(true);
					game::buildings[leftClickID].update_pass();
				}
				movingObject = false;
				movingObjectRestore = false;
			}
		}
	}
}
