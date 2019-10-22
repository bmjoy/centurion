#include <editor>
#include <interface>
#include <object>
#include <global>
#include <picking>
#include <player>

using namespace building;
using namespace unit;
using namespace glb;

namespace editor {

	/* extern variables definitions */

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

	string EditorObjectStringListForm0[NumberOfObjects] = { "" };
	string EditorObjectStringListForm1[NumberOfObjects] = { "" };
	string EditorObjectStringListForm2[NumberOfObjects] = { "" };

	array<Player, 8> playersList;
	Unit unitTemp;
	Building buildingTemp;
	map<int, Building> buildings;
	map<int, Unit> units;

	Editor *EDITOR() { return &myeditor; }

	/* tools */

	void prepareObject(string type, string classname) {
		if (type == "buildings") {
			buildingTemp.set_class(classname);
			buildingTemp.set_id(0);
			buildingTemp.set_player(&playersList[0]);
			buildingTemp.set_position(vec3(0));
			buildingTemp.prepare();
		}
	}
	void insertingObject(string type, string classname) {
		float x = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
		float y = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
		if (type == "buildings") {
			buildingTemp.set_position(vec3(x, y, 0.f));
			buildingTemp.render(false, 0);
		}
	}
	void addObject(string type) {
		if (type == "buildings") {
			float x = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
			float y = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
			buildingTemp.set_position(vec3(x, y, 0.f));
			buildingTemp.set_id(getPickingID());
			buildingTemp.create();
			buildings[getPickingID()] = buildingTemp;
			increasePickingID();
		}
	}
	void renderObjectsPicking() {
		if (!IsWindowOpened && getBoolean("mouse-left")) {
			for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
				bld->second.render(true, 0);
			}
			for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(true, 0);
			}
			leftClickID = get_id();
		}
	}
	void renderObjects() {
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			bld->second.render(false, leftClickID);
		}

		for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
			u->second.render(false, leftClickID);
		}
	}
}