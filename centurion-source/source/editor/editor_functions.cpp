#include <game/editor.h>
#include <interface>
#include <global>
#include <picking.h>
#include <player>
#include <game/strategy.h>
#include <engine.h>
#include <surface>
#include <settings.h>
#include <translationsTable.h>

#include <primitives.h>
#include <terrain.h>

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

	//Unit unitTemp;
	Building* buildingTemp;
	Decoration* decorTemp;

	
	QuestionWindow *Q_WINDOW() { return &myquestionwindow; }

	

	/* tools */
	void prepareObject(string type, string classname) {
		if (type == "buildings") {
			buildingTemp = new Building();
			buildingTemp->SetClassName(classname);
			buildingTemp->SetPickingID(0);
			buildingTemp->SetPlayer(0);
			buildingTemp->set_position(vec3(0));
			buildingTemp->prepare();
		}
		if (type == "decorations") {
			decorTemp = new Decoration();
			decorTemp->SetClassName(classname);
			decorTemp->SetPickingID(0);
			decorTemp->SetPlayer(0);
			decorTemp->set_position(vec3(0));
			decorTemp->prepare();
		}
	}

	void insertingObject(string type, string classname) {
		float x = round(Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
		float y = round(Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
		if (type == "buildings") {
			buildingTemp->set_position(vec3(x, y, 0.f));
			buildingTemp->set_status(false);
			buildingTemp->render(false, 0, !buildingTemp->is_placeable());
			
			//Player will be able to see info about placing status
			if (!buildingTemp->is_independent()) {
				string s = "";
				if (!buildingTemp->is_near_to_independent(&s))
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					if (!buildingTemp->is_placeable())
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
			}
			else {
				if (!buildingTemp->is_placeable())
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
			}
		}
		if (type == "decorations") {
			decorTemp->set_position(vec3(x, y, 0.f));
			decorTemp->render(!decorTemp->is_placeable());
		}
	}

	void addObject(string type) 
	{
		if (type == "buildings") 
		{
			if (buildingTemp->is_placeable())
			{
				unsigned int ID = PickingObject::GetPickingId();
				buildingTemp->SetPickingID(ID);
				buildingTemp->create();
				if (buildingTemp->is_independent()) 
				{
					buildingTemp->set_settlement_name("SETTL_" + buildingTemp->GetName());
				}
				Game::AddGameObject(ID, buildingTemp);
	
				Engine::Mouse::LeftClick = false;
				addingObject = false;
			}
		}
		if (type == "decorations") 
		{
			if (decorTemp->is_placeable()) 
			{
				unsigned int ID = PickingObject::GetPickingId();
				decorTemp->SetPickingID(ID);
				decorTemp->create();
				Game::AddGameObject(ID, decorTemp);
				Engine::Mouse::LeftClick = false;
				addingObject = false;
			}
		}
		Game::Minimap::Update();
	}

	void changeTerrain(int terrainType) {
		float x1 = (Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
		float y1 = (Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
		float type = float(terrainType);
		
		int x = int(round(x1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int y = int(round(y1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		// + mapgen::grid_size * 2; because the map has "borders"

		int j = mapgen::getVertexPos(x, y);
	
		if (mapgen::MapTextures()[j] != type){
			mapgen::MapTextures()[j] = type;
			MapTerrain()->updateTextureBuffer();
			Game::Minimap::Update();
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
		if (Engine::Mouse::LeftHold) {
			// buildings
			if (Game::IsGameObjectNotNull(Picking::leftClickID)) {
				Building* bld = Game::GetGameObjectPtrById(Picking::leftClickID)->AsBuilding();
				movingObjectRestore = false;
				if (!movingObject) {
					movingObjectXPos = bld->get_position().x;
					movingObjectYPos = bld->get_position().y;
					movingObjectStartXMouse = Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition();
					movingObjectStartYMouse = Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition();
				}
				float x1 = (Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
				float y1 = (Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
				float dx = x1 - movingObjectStartXMouse;
				float dy = y1 - movingObjectStartYMouse;

				if (!bld->is_independent()){
					if (!movingObject) bld->clear_pass();
					bld->set_position(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!bld->is_placeable()) {
						string s = "";
						if (!bld->is_near_to_independent(&s)) {
							textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						}
						else
							textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						bld->set_placeable(false);
						movingObjectRestore = true;
					}
					else {
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						bld->set_placeable(true);
					}
				}
				else {
					if (!movingObject) bld->clear_pass();
					bld->set_position(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!bld->is_placeable()) {
						bld->set_placeable(false);
						movingObjectRestore = true;
					}
					else {
						bld->set_placeable(true);
					}
					if (!bld->is_placeable())
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				}
				movingObject = true;
				Game::Minimap::Update();
			}
		}
		else {
			// buildings
			if (Game::IsGameObjectNotNull(Picking::leftClickID)) {
				Building* bld = Game::GetGameObjectPtrById(Picking::leftClickID)->AsBuilding();
				if (movingObjectRestore) {
					bld->set_position(vec3(movingObjectXPos, movingObjectYPos, 0.f));
					bld->clear_pass();
					bld->set_placeable(true);
					bld->update_pass();
				}
				else {
					bld->set_placeable(true);
					bld->update_pass();
				}
				movingObject = false;
				movingObjectRestore = false;
			}
		}
	}
}
