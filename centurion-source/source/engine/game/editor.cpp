#include "editor.h"
#include <mapgen/mapgen.h>
#include <game/strategy.h>
#include <picking.h>
#include <interface>
#include <engine.h>

#include <logger.h>
#include <settings.h>
#include <translationsTable.h>
#include <terrain.h>

#include <hector-lua.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

Editor::Editor(void) {}

void Editor::reset(void)
{
	Game::isCreated = false;
}

void Editor::Create(void)
{
	PickingUI::ResetPicking();
	PickingObject::ResetPicking();

	Strategy::Reset();
	Engine::myWindow::BottomBarHeight = 0.f;
	Engine::myWindow::TopBarHeight = 0.f;

	setMinimapProjectionMatrix();

	Map::Reset();
	editor::EDITOR_UI()->create();

	SelectionRectangle::Create();

	Engine::Mouse::LeftHold = false;

	Engine::Camera::GoToPoint(1.f, 1.f);

	isCreated = true;
	Minimap::Update();
}

void Editor::Run(void)
{
	Picking::leftClickID_UI = 0;

	/* Keyboard control */
	handleKeyboardControls();
	if (!editor::IsWindowOpened) 
	{ // TODO: merge all these in a function in Editor->Editor_functions.cpp
		Engine::Camera::keyboardControl();
	}

	/* If minimap is NOT active */
	if (!Minimap::IsActive()) 
	{
		if (!editor::IsWindowOpened && Engine::Mouse::GetYPosition() < Engine::myWindow::Height - 30.f && !editor::menuIsOpened)
			Engine::Camera::mouseControl();
		viewMatrix = Engine::Camera::calculateViewMatrix();
		projectionMatrix = getCameraProjectionMatrix();

		editor::EDITOR_UI()->render(true);

 		if (Engine::Mouse::LeftClick) 
		{
  			Picking::leftClickID_UI = PickingUI::GetIdFromClick();
		}

		// apply game matrices
		applyGameMatrices(&projectionMatrix, &viewMatrix);

		// picking
		if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) 
			RenderObjectsPicking();

		// rendering
		Map::Render(false);
		RenderObjects();
		//if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) editor::moveObjects();

		// apply menu matrices
		applyMenuMatrices();

		editor::EDITOR_UI()->render(false);
	}

	/* If minimap is active */
	else 
	{
		viewMatrix = mat4(1.0f);
		projectionMatrix = getMinimapProjectionMatrix();

		// editor ui picking */
		editor::EDITOR_UI()->render(true);
		RenderObjectsPicking(); //RIVEDERE QUESTA FUNZIONE

		if (Minimap::IsCreated()) Minimap::Render();

		if (!Minimap::IsCreated()) {
			applyGameMatrices(&projectionMatrix, &viewMatrix);
			Map::Render(false);
			RenderObjects();
			Minimap::Create();
			applyMenuMatrices();
		}

		editor::EDITOR_UI()->render(false);

		if (Picking::leftClickID_UI == 0) GoToPointFromMinimap();
	}

	setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));
}

void Editor::handleKeyboardControls(void) 
{
	using namespace editor;

	//CTRL Hotkeys
	if (!IsWindowOpened) 
	{
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Engine::Keyboard::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL)) 
		{
			if(!Minimap::IsActive())
			{
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_N)) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_O)) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_A)) { TerrainBrushIsActive = false; TerrainBrushWindowIsOpen = false; AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_T)) { AddObjectWindowIsOpen = false; TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen; }
			}
			else
			{
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_S)) 
				{ 
					Game::Map::SaveScenario(currentMapName); 
				}
			}
		}
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_DELETE))
		{
			if (Game::IsGameObjectNotNull(Picking::leftClickID)) 
			{
				Building* b = GObject::GObject::GetObjectByID(Picking::leftClickID)->AsBuilding();
				if (b->IsSelected()) 
				{
					if (b->GetSettlement()->IsIndipendent())
					{
						/*
						if (b->buildingsInSettlementCount() > 0) {
							b->setWaitingToBeErased(true);
							Q_WINDOW()->setQuestion("QUESTION_deleteAll");
						}
						else {
							cout << "[DEBUG] Settlement " << b->GetName() << " deleted!\n";
							b->clear_pass();
							GObject::RemoveGameObject(Picking::leftClickID);
						}
						*/
					}
					else {
						cout << "[DEBUG] Building " << b->GetSingularName() << " deleted!\n";
						b->clear_pass();
						GObject::RemoveGameObject(Picking::leftClickID);
					}
				}
			}
		}
		if (Hector::ConsoleIsActive() == false)
		{
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_SPACE) || Engine::Mouse::MiddleClick)
			{
				if (Minimap::IsActive()) Minimap::Disable();
				else Minimap::Enable(); clearEditorVariables();
				Minimap::IsActive() ? Logger::Info("Minimap ON!") : Logger::Info("Minimap OFF!");
			}
		}
		if (Hector::ConsoleIsActive() == false) 
		{
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_Z)) 
			{
				Map::Wireframe = !Map::Wireframe;
				Map::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
			}
		}
		// Grid
		if (Hector::ConsoleIsActive() == false) 
		{
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_G))
			{
				if (Map::IsGridEnabled()) Map::DisableGrid();
				else Map::EnableGrid();
				Map::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
			}
		}
	}
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE)) 
	{
		if (areWindowsClosed()) 
		{
			clearEditorVariables();
			Engine::Reset();
		}
		else 
		{
			clearEditorVariables();
			//EDITOR_UI()->close_menu();
		}
	}
}

Editor::~Editor(void) { }


/* EDITOR FUNCTIONS */


namespace editor 
{
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
	void prepareObject(const string type, const string classname) 
	{
		if (type == "buildings") {
			buildingTemp = new Building();
			buildingTemp->SetClassName(classname);
			buildingTemp->SetPickingID(0);
			buildingTemp->SetPlayer(0);
			buildingTemp->SetPosition(vec3(0));
			buildingTemp->prepare();
		}
		if (type == "decorations") {
			decorTemp = new Decoration();
			decorTemp->SetClassName(classname);
			decorTemp->SetPickingID(0);
			decorTemp->SetPlayer(0);
			decorTemp->SetPosition(vec3(0));
			decorTemp->prepare();
		}
	}

	void insertingObject(const string type, const string classname)
	{
		float x = round(Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
		float y = round(Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
		if (type == "buildings") {
			buildingTemp->SetPosition(vec3(x, y, 0.f));
			buildingTemp->SetStatus(false);
			buildingTemp->render(false, 0, !buildingTemp->IsPlaceable());

			//Player will be able to see info about placing status
			if (!buildingTemp->GetSettlement()->IsIndipendent()) {
				string s = "";
				
				/*if (!buildingTemp->is_near_to_independent(&s))
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				else*/
				if (!buildingTemp->IsPlaceable())
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				
			}
			else {
				if (!buildingTemp->IsPlaceable())
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
				else
					textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
			}
		}
		if (type == "decorations") {
			decorTemp->SetPosition(vec3(x, y, 0.f));
			decorTemp->render(!decorTemp->is_placeable());
		}
	}

	void addObject(const string type)
	{
		if (type == "buildings")
		{
			/*if (buildingTemp->IsPlaceable())
			{
				unsigned int ID = PickingObject::ObtainPickingID();
				buildingTemp->SetPickingID(ID);
				buildingTemp->create();
				if (buildingTemp->GetSettlement().IsIndipendent())
				{
					buildingTemp->GetSettlement().SetSettlementName("SETTL_" + buildingTemp->GetName());
				}
				Game::AddGameObject(ID, buildingTemp);

				Engine::Mouse::LeftClick = false;
				addingObject = false;
			}*/
		}
		if (type == "decorations")
		{
			/*if (decorTemp->is_placeable())
			{
				unsigned int ID = PickingObject::ObtainPickingID();
				decorTemp->SetPickingID(ID);
				decorTemp->create();
				Game::AddGameObject(ID, decorTemp);
				Engine::Mouse::LeftClick = false;
				addingObject = false;
			}*/
		}
		Game::Minimap::Update();
	}

	void changeTerrain(const int terrainType)
	{
		float x1 = (Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
		float y1 = (Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
		float type = float(terrainType);

		int x = int(round(x1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		int y = int(round(y1 / mapgen::grid_size)) * mapgen::grid_size + mapgen::grid_size * 2;
		// + mapgen::grid_size * 2; because the map has "borders"

		int j = mapgen::getVertexPos(x, y);

		if (mapgen::MapTextures()[j] != type) 
		{
			mapgen::MapTextures()[j] = type;
			MapTerrain()->updateTextureBuffer();
			Game::Minimap::Update();
		}
	}

	void clearEditorVariables(void) 
	{
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

	bool areWindowsClosed(void)
	{
		return !(IsWindowOpened || menuIsOpened);
	}

	void moveObjects(void) 
	{
		if (Engine::Mouse::LeftHold) {
			// buildings
			if (Game::IsGameObjectNotNull(Picking::leftClickID)) {
				Building* bld = GObject::GObject::GetObjectByID(Picking::leftClickID)->AsBuilding();
				movingObjectRestore = false;
				if (!movingObject) {
					movingObjectXPos = bld->GetPosition().x;
					movingObjectYPos = bld->GetPosition().y;
					movingObjectStartXMouse = Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition();
					movingObjectStartYMouse = Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition();
				}
				float x1 = (Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition());
				float y1 = (Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition());
				float dx = x1 - movingObjectStartXMouse;
				float dy = y1 - movingObjectStartYMouse;

				if (!bld->GetSettlement()->IsIndipendent())
				{
					if (!movingObject) bld->clear_pass();
					bld->SetPosition(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!bld->IsPlaceable()) {
						string s = "";
						/*
						if (!bld->is_near_to_independent(&s)) {
							textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_noSettlementsAround"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						}
						else
							textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_impassablePoint"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						*/
						bld->SetPlaceable(false);
						movingObjectRestore = true;
					}
					else {
						textInfo.render_dynamic(TranslationsTable::GetTranslation("EDITOR_canAddStructure"), "tahoma_15px", 10, Engine::myWindow::Height - 50, vec4(255.f), "left", "center");
						bld->SetPlaceable(true);
					}
				}
				else {
					if (!movingObject) bld->clear_pass();
					bld->SetPosition(vec3(movingObjectXPos + dx, movingObjectYPos + dy, 0.f));
					if (!bld->IsPlaceable()) {
						bld->SetPlaceable(false);
						movingObjectRestore = true;
					}
					else {
						bld->SetPlaceable(true);
					}
					if (!bld->IsPlaceable())
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
			//if (Game::IsGameObjectNotNull(Picking::leftClickID)) {
			//	Building* bld = GObject::GObject::GetObjectByID(Picking::leftClickID)->AsBuilding();
			//	if (movingObjectRestore) {
			//		bld->SetPosition(vec3(movingObjectXPos, movingObjectYPos, 0.f));
			//		bld->clear_pass();
			//		bld->SetPlaceable(true);
			//		bld->update_pass();
			//	}
			//	else {
			//		bld->SetPlaceable(true);
			//		bld->update_pass();
			//	}
			//	movingObject = false;
			//	movingObjectRestore = false;
			//}
		}
	}
}






