#include <interface>
#include <picking.h>
#include <engine.h>
#include <mapgen/mapgen.h>
#include <game/editor.h>
#include <game/strategy.h>
#include <classes/object.h>
#include <terrain.h>
#include <file_manager.h>

using namespace std;
using namespace glm;

namespace editor {

	/*--------------------------*/
	/*      NEW MAP WINDOW      */
	/*--------------------------*/

	void NewMapWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::GetPickedObjectName(Picking::leftClickID_UI);

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
				GObject::ResetGameObjects();

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
		string clickName = PickingUI::GetPickedObjectName(Picking::leftClickID_UI);

		// Yes
		if (clickName == "QuestionWindow_Yes") { 
			if (question == "QUESTION_overwriteMap") {
				GObject::ResetGameObjects();

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
					string settl_name = settl->GetSingularName();
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
						GObject::GObject::GetObjectByID(idsToErase[i])->AsBuilding()->clear_pass();
						GObject::RemoveGameObject(idsToErase[i]);
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
					GObject::GObject::GetObjectByID(i)->AsBuilding()->setWaitingToBeErased(false);
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
		string clickName = PickingUI::GetPickedObjectName(Picking::leftClickID_UI);

		if (Picking::leftClickID_UI == 0 || Picking::leftClickID_UI == back_image.ObtainPickingID())
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


	/*--------------------------------*/
	/*      TERRAIN BRUSH WINDOW      */
	/*--------------------------------*/

	void TerrainBrushWindow::picking() {
		GLint mouseX = (GLint)Engine::Mouse::GetXPosition();
		GLint mouseY = (GLint)Engine::Mouse::GetYPosition();
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::GetPickedObjectName(Picking::leftClickID_UI);

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
