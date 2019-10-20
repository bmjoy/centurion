#include <interface>
#include <picking>
#include <engine>
#include <surface>

using namespace glb;
using namespace engine;

namespace editor {

	/*----------------*/
	/*      MENU      */
	/*----------------*/

	void EditorMenu::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID = get_id();
		int pos = (int)((mouseY - getParam("window-height"))*(-1)) / titles["File"].titleHeight;

		//---------------------
		//   RESET 
		//---------------------

		if (leftClickID < minPickingID || leftClickID > maxPickingID) {
			for (int i = 0; i < titles.size(); i++) {
				std::string s = titlesList[i];
				titles[s].isOpened = false;
				menuIsOpened = false;
			}
		}

		if (!IsWindowOpened) {
			
			//---------------------
			//    FILE
			//---------------------

			if (pickingList["File"] == leftClickID) {
				if (pos == 0) {
					titles["File"].isOpened = !titles["File"].isOpened;
					menuIsOpened = !menuIsOpened;
					for (int j = 0; j < titles.size(); j++) {
						std::string s = titlesList[j];
						if (s != "File") titles[s].isOpened = false;
					}
				}
				//---------------------
				//    NEW

				if (pos == 1) {

					NewMapWindowIsOpen = true;
					NewMapResetText = true;
					IsWindowOpened = true;

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}

				//---------------------
				//    OPEN

				if (pos == 2) {

					OpenMapWindowIsOpen = true;
					OpenMapWindowUpdate = true;
					IsWindowOpened = true;

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}

				//---------------------
				//    SAVE

				if (pos == 3) { // save

					saveCurrentScenario(currentMapName);

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}
				//---------------------
				//    EXIT

				if (pos == 4) { // exit				
					ENGINE()->Reset();
					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}
			}

			//---------------------
			//    EDIT
			//---------------------

			if (pickingList["Edit"] == leftClickID) {
				titles["Edit"].isOpened = !titles["Edit"].isOpened;
				menuIsOpened = !menuIsOpened;
				for (int j = 0; j < titles.size(); j++) {
					std::string s = titlesList[j];
					if (s != "Edit") titles[s].isOpened = false;
				}
			}

			//---------------------
			//    TOOLS
			//---------------------

			if (pickingList["Tools"] == leftClickID) {
				titles["Tools"].isOpened = !titles["Tools"].isOpened;
				menuIsOpened = !menuIsOpened;
				for (int j = 0; j < titles.size(); j++) {
					std::string s = titlesList[j];
					if (s != "Tools") titles[s].isOpened = false;
				}
			}

			//---------------------
			//---------------------
		}
	}

    bool EditorMenu::isHover(glm::vec2 pos, int w, int h) {

        if ((getParam("mouse-x-position") > pos.x) && (getParam("mouse-x-position") < pos.x + w) && (getParam("mouse-y-position") > pos.y) && (getParam("mouse-y-position") < pos.y + h)) {
            return true;
        }
        else {
            return false;
        }
    }

	/*---------------------------*/
	/*      OPEN MAP WINDOW      */
	/*---------------------------*/

	void OpenMapWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID = get_id();
		string clickName = getPickedObjectName(leftClickID);
		int pos = (int)((mouseY - startY + map_list.padding_top)*(-1)) / (int)map_list.option_height;

		//Find a better way to clickName != "OpenMapWindow_open") selectedID = -1, by including elements list too
		//otherwise selectedID will be reset to -1 every second and double click won't work properly
		//if (clickName != "OpenMapWindow_open") selectedID = -1;

		if (selectedID != -1 && selectedID == pos && hasDoubleClicked()) {
			cout << "DEBUG: You've chosen the following scenario to open: " + availableScenarios[selectedID] << endl;
			openScenario(availableScenarios[selectedID]);
			currentMapName = availableScenarios[selectedID];
			obj::MapTerrain()->updateHeightsBuffer();
			obj::MapTerrain()->updateTextureBuffer();
			OpenMapWindowIsOpen = false;
			IsWindowOpened = false;
		}
		cout << selectedID << "\n";
		if (leftClickID == map_list.pickingID) {
			if (pos >= 0) {
				selectedID = pos;
			}
		}
		

		if (clickName == "OpenMapWindow_close") { // CLOSE
			OpenMapWindowIsOpen = false;
			IsWindowOpened = false;
		}

		if (clickName == "OpenMapWindow_open") { // OPEN
			if (selectedID != -1) {
				cout << "DEBUG: You've chosen the following scenario to open: " + availableScenarios[selectedID] << endl;
				openScenario(availableScenarios[selectedID]);
				currentMapName = availableScenarios[selectedID];
				obj::MapTerrain()->updateHeightsBuffer();
				obj::MapTerrain()->updateTextureBuffer();
				OpenMapWindowIsOpen = false;
				IsWindowOpened = false;
			}
		}
	}

	/*--------------------------*/
	/*      NEW MAP WINDOW      */
	/*--------------------------*/

	void NewMapWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		int clickId = get_id();
		string clickName = getPickedObjectName(clickId);

		text_input.active();

		if (clickName == "NewMapWindow_close") { // CLOSE
			NewMapWindowIsOpen = false;
			IsWindowOpened = false;
		}

		if (clickName == "NewMapWindow_textclick") {
			text_input.active();
		}
		else {
			text_input.active(false);
		}

		if (clickName == "NewMapWindow_create") { // CREATE

			cout << "DEBUG: You've set the following map name: " + text_input.get_text() << endl;
			
			mapgen::reset_map();
			obj::MapTerrain()->updateHeightsBuffer();
			obj::MapTerrain()->updateTextureBuffer();
			NewMapWindowIsOpen = false;
			IsWindowOpened = false;
		}
	}
};
