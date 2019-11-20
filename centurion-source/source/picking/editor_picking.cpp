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

		if (!IsWindowOpened && !addingObject) {
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
					clearEditorVariables();
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
					clearEditorVariables();
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

	void EditorMenu::closeMenu() {
		for (int i = 0; i < titles.size(); i++) {
			std::string s = titlesList[i];
			titles[s].isOpened = false;
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
				cout << "[DEBUG] You've chosen the following scenario to open: " + availableScenarios[selectedID] << endl;
				game::buildings.clear();
				game::units.clear();
				
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
			cout << "[DEBUG] You've set the following map name: " + text_input.get_text() << endl;
			currentMapName = text_input.get_text();

			NewMapWindowIsOpen = false;

			//Does the map folder already exist?
			if (!folderExists("scenarios/" + currentMapName)) {
				game::buildings.clear();
				game::units.clear();

				mapgen::reset_map();
				obj::MapTerrain()->updateHeightsBuffer();
				obj::MapTerrain()->updateTextureBuffer();
				clearEditorVariables();
				saveCurrentScenario(currentMapName);
			}
			else
				Q_WINDOW()->setQuestion("QUESTION_overwriteMap");
		}
	}

	/*--------------------------*/
	/*     QUESTION WINDOW      */
	/*--------------------------*/

	void QuestionWindow::picking() {
		GLint mouseX = (GLint)getParam("mouse-x-position");
		GLint mouseY = (GLint)getParam("mouse-y-position");
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);

		// Yes
		if (clickName == "QuestionWindow_Yes") { 
			if (question == "QUESTION_overwriteMap") {
				game::buildings.clear();
				game::units.clear();

				mapgen::reset_map();
				obj::MapTerrain()->updateHeightsBuffer();
				obj::MapTerrain()->updateTextureBuffer();
				saveCurrentScenario(currentMapName);
				cout << "[DEBUG] Map " + currentMapName + " has been overwritten successfully!" << endl;
			}
			if (question == "QUESTION_deleteAll") {
				vector<int> idsToErase;
				for (map<int, Building*>::iterator bld = game::central_buildings.begin(); bld != game::central_buildings.end(); bld++) {
					int settl_id = bld->first;
					Building* settl = bld->second;
					string settl_name = bld->second->get_name();
					if (settl->getWaitingToBeErased()) {
						idsToErase.push_back(settl_id);
						for(int i = 0; i < settl->buildingsInSettlementIds().size(); i++){
							idsToErase.push_back(settl->buildingsInSettlementIds()[i]);
						}
					}
				}
				if (idsToErase.size() > 0){
					for (int i = 0; i < idsToErase.size(); i++) {
						game::buildings[idsToErase[i]].clear_pass();
						game::buildings.erase(idsToErase[i]);
					}
					game::central_buildings.erase(idsToErase[0]);
					cout << "[DEBUG]: Settlement " << idsToErase[0] << " completly erased!\n";
				}
			}
			clearEditorVariables();
		}

		// No
		if (clickName == "QuestionWindow_No") {
			if (question == "QUESTION_deleteAll") {
				for (map<int, Building*>::iterator bld = game::central_buildings.begin(); bld != game::central_buildings.end(); bld++) {
					game::buildings[bld->first].setWaitingToBeErased(false);
				}
			}
			clearEditorVariables();
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

		if (leftClickID_UI == 0 || leftClickID_UI == back_image.getPickingID())
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

		if (clickName == "AddObjWindow_leftarrow") { // LEFT ARROW
			objectForms[2].select_previous();
			update();
		}

		if (clickName == "AddObjWindow_rightarrow") { // RIGHT ARROW
			objectForms[2].select_next();
			update();
		}

		if (clickName == "AddObjWindow_add") { // ADD
			addingObject = true;
			setBoolean("mouse-left", false);
			setParam("mouse-x-leftclick", 0.f);
			setParam("mouse-y-leftclick", 0.f);
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

		text_input[0].active(clickName == "PropertiesWindow_name");
		if (building_pointer->is_central_building())
			text_input[1].active(clickName == "PropertiesWindow_settl_name");

		if (clickName == "PropertiesWindow_cancel") { // CANCEL
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
			setBoolean("mouse-left", false);
		}
		if (clickName == "PropertiesWindow_apply") { // APPLY
			if (type == "building"){
				building_pointer->set_name(text_input[0].get_text());
				if (building_pointer->is_central_building())
					building_pointer->set_settlement_name(text_input[1].get_text());
			}
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
			setBoolean("mouse-left", false);
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
			for (int i = 0; i < forms.size(); i++) {
				forms[i].close();
			}

		for (int i = 0; i < forms.size(); i++) {
			if (clickName == "TerrainBrush_form_"+i) {
				int j = forms[i].get_clicked_option();
				forms[i].open_close();
				if (j > 0) {
					forms[i].select_option(j);
					TerrainBrushIsActive = true;
					selBrush = forms[i].selectedText;
				}
			}
		}
	}
};
