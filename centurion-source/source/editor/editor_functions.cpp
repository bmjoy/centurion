#include <game/editor.h>
#include <interface>
#include <global>
#include <picking>
#include <player>
#include <game/strategy.h>
#include <engine/engine.h>
#include <engine/mouse.h>
#include <engine/camera.h>
#include <engine/window.h>
#include <surface>
#include <settings.h>


using namespace engine;


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
			buildingTemp->set_class(classname);
			buildingTemp->set_id(0);
			buildingTemp->set_player(0);
			buildingTemp->set_position(vec3(0));
			buildingTemp->prepare();
		}
		if (type == "decorations") {
			decorTemp = new Decoration();
			decorTemp->set_class(classname);
			decorTemp->set_id(0);
			decorTemp->set_player(0);
			decorTemp->set_position(vec3(0));
			decorTemp->prepare();
		}
	}

	void insertingObject(string type, string classname) {
		float x = round(engine::Mouse::GetXPosition() * engine::myWindow::WidthZoomed / myWindow::Width + engine::Camera::GetXPosition());
		float y = round(engine::Mouse::GetYPosition() * engine::myWindow::HeightZoomed / myWindow::Height + engine::Camera::GetYPosition());
		if (type == "buildings") {
			buildingTemp->set_position(vec3(x, y, 0.f));
			buildingTemp->set_status(false);
			buildingTemp->render(false, 0, !buildingTemp->is_placeable());
			
			//Player will be able to see info about placing status
			if (!buildingTemp->is_independent()) {
				string s = "";
				if (!buildingTemp->is_near_to_independent(&s))
					textInfo.render_dynamic(getTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					if (!buildingTemp->is_placeable())
						textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
			}
			else {
				if (!buildingTemp->is_placeable())
					textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
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
				unsigned int ID = getPickingID(); 
				buildingTemp->set_id(ID);
				buildingTemp->create();
				if (buildingTemp->is_independent()) 
				{
					buildingTemp->set_settlement_name("SETTL_" + buildingTemp->get_name());
				}
				Game::AddGameObject(ID, buildingTemp);
	
				Mouse::LeftClick = false;
				addingObject = false;
			}
		}
		if (type == "decorations") 
		{
			if (decorTemp->is_placeable()) 
			{
				unsigned int ID = getPickingID(); 
				decorTemp->set_id(ID);
				decorTemp->create();
				Game::AddGameObject(ID, decorTemp);
				Mouse::LeftClick = false;
				addingObject = false;
			}
		}
		Game::Minimap::Update();
	}

	void changeTerrain(int terrainType) {
		float x1 = (engine::Mouse::GetXPosition() * engine::myWindow::WidthZoomed / myWindow::Width + engine::Camera::GetXPosition());
		float y1 = (engine::Mouse::GetYPosition() * engine::myWindow::HeightZoomed / myWindow::Height + engine::Camera::GetYPosition());
		float type = float(terrainType);
		
		int x = int(round(x1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int y = int(round(y1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		// + mapgen::grid_size * 2; because the map has "borders"

		int j = mapgen::getVertexPos(x, y);
	
		if (mapgen::MapTextures()[j] != type){
			mapgen::MapTextures()[j] = type;
			obj::MapTerrain()->updateTextureBuffer();
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
		if (Mouse::LeftHold) {
			// buildings
			if (Game::IsGameObjectNotNull(leftClickID)) {
				Building* bld = Game::GetGameObjectPtrById(leftClickID)->AsBuilding();
				movingObjectRestore = false;
				if (!movingObject) {
					movingObjectXPos = bld->get_position().x;
					movingObjectYPos = bld->get_position().y;
					movingObjectStartXMouse = engine::Mouse::GetXPosition() * engine::myWindow::WidthZoomed / myWindow::Width + engine::Camera::GetXPosition();
					movingObjectStartYMouse = engine::Mouse::GetYPosition() * engine::myWindow::HeightZoomed / myWindow::Height + engine::Camera::GetYPosition();
				}
				float x1 = (engine::Mouse::GetXPosition() * engine::myWindow::WidthZoomed / myWindow::Width + engine::Camera::GetXPosition());
				float y1 = (engine::Mouse::GetYPosition() * engine::myWindow::HeightZoomed / myWindow::Height + engine::Camera::GetYPosition());
				float dx = x1 - movingObjectStartXMouse;
				float dy = y1 - movingObjectStartYMouse;

				if (!bld->is_independent()){
					if (!movingObject) bld->clear_pass();
					bld->set_position(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!bld->is_placeable()) {
						string s = "";
						if (!bld->is_near_to_independent(&s)) {
							textInfo.render_dynamic(getTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
						}
						else
							textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
						bld->set_placeable(false);
						movingObjectRestore = true;
					}
					else {
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
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
						textInfo.render_dynamic(getTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
					else
						textInfo.render_dynamic(getTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, myWindow::Height - 50, vec4(255.f), "left", "center");
				}
				movingObject = true;
				Game::Minimap::Update();
			}
		}
		else {
			// buildings
			if (Game::IsGameObjectNotNull(leftClickID)) {
				Building* bld = Game::GetGameObjectPtrById(leftClickID)->AsBuilding();
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
