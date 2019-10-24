#include <interface>
#include <picking>
#include <engine>
#include <surface>
#include <editor>
#include <game>
#include <object>

using namespace glb;
using namespace engine;

namespace editor {

	/*----------------*/
	/*      MENU      */
	/*----------------*/

	void EditorMenu::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID_UI = get_id();
		int pos = (int)((mouseY - getParam("window-height"))*(-1)) / titles["File"].titleHeight;

		//---------------------
		//   RESET 
		//---------------------

		if (leftClickID_UI < minPickingID || leftClickID_UI > maxPickingID) {
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

			if (pickingList["File"] == leftClickID_UI) {
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
					AddObjectWindowIsOpen = false;
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
					AddObjectWindowIsOpen = false;
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
					//setBoolean("mouse-left-pressed", false);
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

			if (pickingList["Edit"] == leftClickID_UI) {
				if (pos == 0){
					titles["Edit"].isOpened = !titles["Edit"].isOpened;
					menuIsOpened = !menuIsOpened;
					for (int j = 0; j < titles.size(); j++) {
						std::string s = titlesList[j];
						if (s != "Edit") titles[s].isOpened = false;
					}
				}

				//---------------------
				//    UPDATE NORMALS

				if (pos == 1) { // Update normals
					mapgen::updateAllNormals();
					obj::MapTerrain()->updateHeightsBuffer();

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}
			}

			//---------------------
			//    TOOLS
			//---------------------

			if (pickingList["Tools"] == leftClickID_UI) {
				if (pos == 0) {
					titles["Tools"].isOpened = !titles["Tools"].isOpened;
					menuIsOpened = !menuIsOpened;
					for (int j = 0; j < titles.size(); j++) {
						std::string s = titlesList[j];
						if (s != "Tools") titles[s].isOpened = false;
					}
				}

				//---------------------
				//    ADD OBJECT

				if (pos == 1) { // Add Object
					AddObjectWindowIsOpen = !AddObjectWindowIsOpen;

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}

				//---------------------
				//    TERRAIN BRUSH

				if (pos == 2) { // Terrain Brush
					TerrainBrushIsActive = !TerrainBrushWindowIsOpen;
					TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen;

					for (int i = 0; i < titles.size(); i++) {
						std::string s = titlesList[i];
						titles[s].isOpened = false;
					}
					menuIsOpened = false;
				}
			}

			//---------------------
			//---------------------
		}
	}

	//void EditorMenu::closeMenu() {
	/*	for (int i = 0; i < titles.size(); i++) {
			std::string s = titlesList[i];
			titles[s].isOpened = false;
		}*/
	//}

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
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);
		int pos = (int)((mouseY - startY + map_list.padding_top)*(-1)) / (int)map_list.option_height;

		if (leftClickID_UI == 0) selectedID = -1;

		if (leftClickID_UI == map_list.pickingID) {
			if (pos >= 0) {
				selectedID = pos;
			}
		}
		
		if (clickName == "OpenMapWindow_close") { // CLOSE
			OpenMapWindowIsOpen = false;
			IsWindowOpened = false;
		}

		if (clickName == "OpenMapWindow_open" || (selectedID == pos && hasDoubleClicked())) { // OPEN
			if (selectedID != -1) {
				cout << "DEBUG: You've chosen the following scenario to open: " + availableScenarios[selectedID] << endl;
				game::buildings = { };
				game::units = { };
				
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
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		if (clickName == "NewMapWindow_close") { // CLOSE
			currentMapName = text_input.get_text();

			NewMapWindowIsOpen = false;
			IsWindowOpened = false;
		}

		text_input.active(clickName == "NewMapWindow_textclick");

		if (clickName == "NewMapWindow_create") { // CREATE
			cout << "DEBUG: You've set the following map name: " + text_input.get_text() << endl;
			currentMapName = text_input.get_text();
			
			game::buildings = { };
			game::units = { };

			mapgen::reset_map();
			obj::MapTerrain()->updateHeightsBuffer();
			obj::MapTerrain()->updateTextureBuffer();
			NewMapWindowIsOpen = false;
			IsWindowOpened = false;
			//This section should be replaced with a question asking for overwriting
			struct stat info;
			string sPath = "scenarios/" + currentMapName;
			if (stat(sPath.c_str(), &info) == 0) MessageBox(NULL, "Selected name already exists.\nMap will be overwritten.", "Centurion", MB_ICONINFORMATION);
			saveCurrentScenario(currentMapName);
		}
	}

	/*--------------------------*/
	/*      ADD OBJ WINDOW      */
	/*--------------------------*/

	void AddObjectWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		if (leftClickID_UI == 0)
			for (int j = 0; j < 3; j++)
				objectForms[j].close();

		for (int j = 0; j < 3; j++) {
			if (clickName == "AddObjWindow_form" + to_string(j)) {
				int i = objectForms[j].get_clicked_option();
				objectForms[j].open_close();
				if (i > 0) {
					objectForms[j].select_option(i);
					if (j == 0)
						AddObjectWindowUpdateForm1and2 = true;
					if (j == 1)
						AddObjectWindowUpdateForm2 = true;
					if (j == 2)
						update();
				}
				for (int k = 0; k < 3; k++)
					if (k != j)
						objectForms[k].close();
			}
		}

		if (clickName == "AddObjWindow_close") { // CLOSE
			AddObjectWindowIsOpen = false;
			IsWindowOpened = false;
		}

		if (clickName == "AddObjWindow_leftarrow") { // CLOSE
			objectForms[2].select_previous();
			update();
		}

		if (clickName == "AddObjWindow_rightarrow") { // CLOSE
			objectForms[2].select_next();
			update();
		}

		if (clickName == "AddObjWindow_add") { // CLOSE
			addingObject = true;
			setBoolean("mouse-left", false);
		}
	}

	/*-----------------------------*/
	/*      PROPERTIES WINDOW      */
	/*-----------------------------*/

	void PropertiesWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		text_input.active(clickName == "PropertiesWindow_textclick");

		if (clickName == "PropertiesWindow_abort") { // ABORT
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
		}
		if (clickName == "PropertiesWindow_apply") { // APPLY
			if (type == "building")
				building_pointer->set_name(text_input.get_text());
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
		}
	}

	/*--------------------------------*/
	/*      TERRAIN BRUSH WINDOW      */
	/*--------------------------------*/

	void TerrainBrushWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		if (leftClickID_UI == 0)
			form1.close();

		if (clickName == "TerrainBrush_form1") {
			int i = form1.get_clicked_option();
			form1.open_close();
			if (i > 0) {
				form1.select_option(i);
				TerrainBrushIsActive = true;
			}
		}
	}
};
