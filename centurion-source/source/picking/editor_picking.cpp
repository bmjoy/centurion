#include <interface>
#include <picking.h>
#include <engine.h>
#include <mapgen/mapgen.h>
#include <game/editor.h>
#include <game/strategy.h>
#include <classes/object.h>

#include <terrain.h>


#include <file_manager.h>





namespace editor {

	/*---------------------------*/
	/*      OPEN MAP WINDOW      */
	/*---------------------------*/

	void OpenMapWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);
		int pos = (int)((mouseY - startY + map_list.padding_top)*(-1)) / (int)map_list.option_height;

		if (Picking::leftClickID_UI == 0) selectedID = -1;

		if (Picking::leftClickID_UI == map_list.pickingID) {
			if (pos >= 0) {
				selectedID = pos;
			}
		}
		
		if (clickName == "OpenMapWindow_close") { // CLOSE
			OpenMapWindowIsOpen = false;
			IsWindowOpened = false;
		}

		if (clickName == "OpenMapWindow_open" || (selectedID == pos && Picking::hasDoubleClicked())) { // OPEN
			if (selectedID != -1) {
				cout << "[DEBUG] You've chosen the following scenario to open: " + availableScenarios[selectedID] << endl;
				Game::ResetGameObjects();

				Game::Map::LoadScenario(availableScenarios[selectedID]);

				currentMapName = availableScenarios[selectedID];
				MapTerrain()->updateHeightsBuffer();
				MapTerrain()->updateTextureBuffer();

				OpenMapWindowIsOpen = false;
				IsWindowOpened = false;
			}
		}
	}

	/*--------------------------*/
	/*      NEW MAP WINDOW      */
	/*--------------------------*/

	void NewMapWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

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
			if (FileManager::CheckIfFolderExists("scenarios/" + currentMapName)) {
				Game::ResetGameObjects();

				mapgen::reset_map();
				MapTerrain()->updateHeightsBuffer();
				MapTerrain()->updateTextureBuffer();
				clearEditorVariables();
				Game::Map::SaveScenario(currentMapName);
			}
			else
				Q_WINDOW()->setQuestion("QUESTION_overwriteMap");
		}
	}

	/*--------------------------*/
	/*     QUESTION WINDOW      */
	/*--------------------------*/

	void QuestionWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

		// Yes
		if (clickName == "QuestionWindow_Yes") { 
			if (question == "QUESTION_overwriteMap") {
				Game::ResetGameObjects();

				mapgen::reset_map();
				MapTerrain()->updateHeightsBuffer();
				MapTerrain()->updateTextureBuffer();
				Game::Map::SaveScenario(currentMapName);
				cout << "[DEBUG] Map " + currentMapName + " has been overwritten successfully!" << endl;
			}
			if (question == "QUESTION_deleteAll") {
				vector<int> idsToErase;
				vector<Building*> indipBuildings = Game::GetListOfIndipendentBuildings();
				
				for (int i = 0; i < indipBuildings.size(); i++) {
					Building* settl = indipBuildings[i]->AsBuilding();
					string settl_name = settl->GetName();
					if (settl->getWaitingToBeErased()) {
						idsToErase.push_back(settl->GetPickingID());
						/*
						for (int j = 0; j < settl->buildingsInSettlementIds().size(); j++) {
							idsToErase.push_back(settl->buildingsInSettlementIds()[j]);
						}
						*/
					}
				}

				if (idsToErase.size() > 0){
					for (int i = 0; i < idsToErase.size(); i++) {
						Game::GetGameObjectPtrById(idsToErase[i])->AsBuilding()->clear_pass();
						Game::RemoveGameObject(idsToErase[i]);
					}
					cout << "[DEBUG]: Settlement " << idsToErase[0] << " completly erased!\n";
				}
			}
			clearEditorVariables();
		}

		// No
		if (clickName == "QuestionWindow_No") {
			if (question == "QUESTION_deleteAll") {

				vector<Building*> indipBuildings = Game::GetListOfIndipendentBuildings();
				for (int i = 0; i < indipBuildings.size(); i++) {
					Game::GetGameObjectPtrById(i)->AsBuilding()->setWaitingToBeErased(false);
				}
			}
			clearEditorVariables();
		}
	}

	/*--------------------------*/
	/*      ADD OBJ WINDOW      */
	/*--------------------------*/

	void AddObjectWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

		if (Picking::leftClickID_UI == 0 || Picking::leftClickID_UI == back_image.obtainPickingID())
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
			if (Game::Minimap::IsActive() == false){
				addingObject = true;
				Engine::Mouse::LeftClick = false;
			}
		}
	}

	/*-----------------------------*/
	/*      PROPERTIES WINDOW      */
	/*-----------------------------*/

	void PropertiesWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

		text_input[0].active(clickName == "PropertiesWindow_name");
		if (building_pointer->GetSettlement().IsIndipendent())
			text_input[1].active(clickName == "PropertiesWindow_settl_name");

		if (clickName == "PropertiesWindow_cancel") { // CANCEL
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
			Engine::Mouse::LeftClick = false;
		}
		if (clickName == "PropertiesWindow_apply") { // APPLY
			if (type == "building"){
				building_pointer->SetName(text_input[0].get_text());
				if (building_pointer->GetSettlement().IsIndipendent())
					building_pointer->GetSettlement().SetSettlementName(text_input[1].get_text());
			}
			PropertiesWindowIsOpen = false;
			IsWindowOpened = false;
			Engine::Mouse::LeftClick = false;
		}
	}

	/*--------------------------------*/
	/*      TERRAIN BRUSH WINDOW      */
	/*--------------------------------*/

	void TerrainBrushWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);

		if (Picking::leftClickID_UI == 0)
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
					selBrush = forms[i].selectedText.substr(12);
				}
			}
		}
	}
};
