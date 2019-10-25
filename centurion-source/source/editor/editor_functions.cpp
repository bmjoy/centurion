#include <editor>
#include <interface>
#include <object>
#include <global>
#include <picking>
#include <player>
#include <game>
#include <engine>
#include <surface>

using namespace building;
using namespace unit;
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
	bool addingObject = false;

	string EditorObjectStringListForm0[NumberOfObjects] = { "" };
	string EditorObjectStringListForm1[NumberOfObjects] = { "" };
	string EditorObjectStringListForm2[NumberOfObjects] = { "" };

	Unit unitTemp;
	Building buildingTemp;

	Editor *EDITOR() { return &myeditor; }

	void Editor::handleKeyboardControls() {
		//CTRL Hotkeys
		if (KeyCode[GLFW_KEY_LEFT_CONTROL] || KeyCode[GLFW_KEY_RIGHT_CONTROL]) {
			if (KeyCode[GLFW_KEY_N]) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
			if (KeyCode[GLFW_KEY_O]) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
			if (KeyCode[GLFW_KEY_S]) { saveCurrentScenario(currentMapName); }
			if (KeyCode[GLFW_KEY_A]) { AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
			if (KeyCode[GLFW_KEY_T]) { TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen;
			}
		}
		if (KeyCode[GLFW_KEY_ESCAPE]) {
			//if () {
			engine::ENGINE()->Reset();
			//}
			/*else {
				//Give access to titles variable in editor_picking and call function "closeMenu"
				menuIsOpened = false;
			}*/
		}
		if (KeyCode[GLFW_KEY_SPACE] || getBoolean("mouse-middle")) {
			game::gameMinimapStatus = !game::gameMinimapStatus;
			//KeyCode[GLFW_KEY_SPACE] = false;
			game::gameMinimapStatus ? std::cout << "DEBUG: Minimap ON!\n" : std::cout << "DEBUG: Minimap OFF!\n";
		}
		if (KeyCode[GLFW_KEY_Z]) {
			setBoolean("wireframe", !getBoolean("wireframe"));
			//KeyCode[GLFW_KEY_Z] = false;
			getBoolean("wireframe") ? std::cout << "DEBUG: Wireframe ON!\n" : std::cout << "DEBUG: Wireframe OFF! \n";
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
	}
	void insertingObject(string type, string classname) {
		float x = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
		float y = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
		if (type == "buildings") {
			buildingTemp.set_position(vec3(x, y, 0.f));
			buildingTemp.render(false, 0, !buildingTemp.is_placeable());
		}
	}
	void addObject(string type) {
		if (type == "buildings") {
			if (buildingTemp.is_placeable()){
				buildingTemp.set_id(getPickingID());
				buildingTemp.create();
				game::buildings[getPickingID()] = buildingTemp;
				increasePickingID();
				setBoolean("mouse-left", false);
				addingObject = false;
			}
		}
	}
	void changeTerrain(int terrainType) {
		float x1 = (getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position"));
		float y1 = (getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position"));
		float type = float(terrainType);
		
		int x = int(round(x1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int y = int(round(y1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int j = mapgen::getVertexPos(x, y);
	
		if (mapgen::MapTextures()[j] != type){
			mapgen::MapTextures()[j] = type;
			obj::MapTerrain()->updateTextureBuffer();
		}
	}
}