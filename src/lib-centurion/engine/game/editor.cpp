#include "editor.h"

#include "interface/editorUi.h"

#include <mapgen/mapgen.h>
#include <game/strategy.h>
#include <picking.h>
#include <engine.h>
#include <menu/menu.h>
#include <logger.h>
#include <settings.h>
#include <translationsTable.h>
#include <terrain.h>
#include <game/interface/editorMenuBar.h>

#include <hector-lua.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;


#pragma region static variables:
std::vector<std::array<std::string, 3>> Editor::editorTree;
std::vector<std::string> Editor::editorTreeList1;
std::vector<std::string> Editor::editorTreeList2;
std::vector<std::string> Editor::editorTreeList3;
GObject* Editor::tmpObject = nullptr;
#pragma endregion

Editor::Editor(void) {}

void Editor::Create(void)
{
	PickingUI::ResetPicking();
	PickingObject::ResetPicking();

	ResetGame();
	Engine::myWindow::BottomBarHeight = 0.f;
	Engine::myWindow::TopBarHeight = 0.f;

	setMinimapProjectionMatrix();

	Map::Reset();
	EditorUI::Create();

	SelectionRectangle::Create();

	Engine::Mouse::LeftClick = false;
	Engine::Mouse::RightClick = false;
	Engine::Mouse::LeftHold = false;
	
	Editor::tmpObject = nullptr;
	
	Engine::Camera::GoToPoint(1.f, 1.f);

	isCreated = true;
	Minimap::Update();
}

void Editor::AddEditorTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3)
{
	std::array<std::string, 3> element = { filter1, filter2, filter3 };
	Editor::editorTree.push_back(element);
}

std::vector<std::string>* Editor::GetEditorTreeList1(void)
{
	Editor::editorTreeList1.clear();
	for (auto i : Editor::editorTree)
	{
		if (std::find(Editor::editorTreeList1.begin(), Editor::editorTreeList1.end(), i[0]) != Editor::editorTreeList1.end())
			continue;
		Editor::editorTreeList1.push_back(i[0]);
	}
	std::sort(Editor::editorTreeList1.begin(), Editor::editorTreeList1.end());
	return &Editor::editorTreeList1;
}

std::vector<std::string>* Editor::GetEditorTreeList2(const std::string filter1)
{
	Editor::editorTreeList2.clear();
	for (auto i : Editor::editorTree)
	{
		if (std::find(Editor::editorTreeList2.begin(), Editor::editorTreeList2.end(), i[1]) != Editor::editorTreeList2.end())
			continue;
		if (i[0] != filter1) continue;
		Editor::editorTreeList2.push_back(i[1]);
	}
	std::sort(Editor::editorTreeList2.begin(), Editor::editorTreeList2.end());
	return &Editor::editorTreeList2;
}

std::vector<std::string>* Editor::GetEditorTreeList3(const std::string filter1, const std::string filter2)
{
	Editor::editorTreeList3.clear();
	for (auto i : Editor::editorTree)
	{
		if (std::find(Editor::editorTreeList3.begin(), Editor::editorTreeList3.end(), i[1]) != Editor::editorTreeList3.end())
			continue;
		if (i[0] != filter1 || i[1] != filter2) continue;
		Editor::editorTreeList3.push_back(i[2]);
	}
	std::sort(Editor::editorTreeList3.begin(), Editor::editorTreeList3.end());
	return &Editor::editorTreeList3;
}

void Editor::InsertingObject(std::string type, std::string className)
{
	if (type.empty() == false && className.empty() == false) 
	{
		if (type == "buildings")
		{
			Editor::tmpObject = new Building();
		}
		else if (type == "units")
		{
			Editor::tmpObject = new Unit();
		}
		else if (type == "decorations")
		{
			Editor::tmpObject = new Decoration();
		}
		else
			return;

		if (Editor::tmpObject->Create(className, true) == false)
		{
			delete Editor::tmpObject;
			Editor::tmpObject = nullptr;
			return;
		}
		Editor::tmpObject->SetPlayer(1);
		EditorMenuBar::Hide();
		return;
	}

	if (Editor::tmpObject == nullptr) return;

	if (Engine::Mouse::RightClick)
	{
		delete Editor::tmpObject;
		Editor::tmpObject = nullptr;
		EditorMenuBar::Show();
		Engine::Mouse::RightClick = false;
	}

	if (Engine::Mouse::LeftClick)
	{
		Game::CreateObject(tmpObject->GetClassName(), tmpObject->GetPosition().x, tmpObject->GetPosition().y, 1);
		delete Editor::tmpObject;
		Editor::tmpObject = nullptr;
		EditorMenuBar::Show();
		Engine::Mouse::LeftClick = false;
	}

	if (Editor::tmpObject == nullptr) return;

	Editor::tmpObject->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 0.f));
	Editor::tmpObject->Render(false, 0, true);
}

void Editor::ShiftSelectedObject(void)
{
	GObject *obj = Game::GetSelectedObject();
	if (obj != nullptr)
	{
		if (Engine::Mouse::LeftHold == true)
		{
			obj->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 0.f));
			obj->Render(false, 0, true);
		}
	}
}

bool Editor::IsInsertingObject(void)
{
	return (Editor::tmpObject != nullptr);
}

void Editor::Close(void)
{
	isCreated = false;
	Engine::SetEnvironment("menu");
	Menu::Reset();
}

void Editor::Run(void)
{
	if (IsCreated() == false)
	{
		//Audio()->MusicStop();
		Create();
		Logger::Info("Editor has been created!");
	}


	Picking::leftClickID_UI = 0;

	/* Keyboard control */
	handleKeyboardControls();

	/* If minimap is NOT active */
	if (!Minimap::IsActive())
	{
		Engine::Camera::mouseControl();
		Engine::Camera::keyboardControl();

		viewMatrix = Engine::Camera::calculateViewMatrix();
		projectionMatrix = getCameraProjectionMatrix();

		// PICKING UI RENDERING
		EditorUI::Render(true);

		if (Engine::Mouse::LeftClick)
		{
			Picking::leftClickID_UI = PickingUI::GetIdFromClick();
		}

		// apply game matrices
		applyGameMatrices(&projectionMatrix, &viewMatrix);

		// PICKING OBJECTS RENDERING
		//if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) 
		{
			RenderObjectsPicking();
		}

		// NORMAL RENDERING
		Map::Render(false);
		Game::RenderObjects();
		Editor::InsertingObject();
		Editor::ShiftSelectedObject();

		//if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) editor::moveObjects();

		// apply menu matrices
		applyMenuMatrices();

		EditorUI::Render(false);
	}

	/* If minimap is active */
	else
	{
		viewMatrix = mat4(1.0f);
		projectionMatrix = getMinimapProjectionMatrix();

		// PICKING RENDERING
		EditorUI::Render(true);

		applyGameMatrices(&projectionMatrix, &viewMatrix);

		RenderObjectsPicking(); //RIVEDERE QUESTA FUNZIONE

		// NORMAL RENDERING 
		Minimap::RenderMapAndObjects();

		applyMenuMatrices();

		Minimap::RenderRectangle();
		EditorUI::Render(false);

		if (Picking::leftClickID_UI == 0) GoToPointFromMinimap();
	}

	setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));
}

void Editor::handleKeyboardControls(void)
{
	//CTRL Hotkeys
	//if (!IsWindowOpened) 
	{
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Engine::Keyboard::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL))
		{
			if (!Minimap::IsActive())
			{
				//if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_N)) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
				//if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_O)) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
				//if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_A)) { TerrainBrushIsActive = false; TerrainBrushWindowIsOpen = false; AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
				//if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_T)) { AddObjectWindowIsOpen = false; TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen; }
			}
			else
			{
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_S))
				{
					//Game::Map::SaveScenario(currentMapName); 
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
				else Minimap::Enable();
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
		Close();
	}
}

Editor::~Editor(void)
{
	if (Editor::tmpObject != nullptr)
	{
		delete Editor::tmpObject;
		Editor::tmpObject = nullptr;
	}
}
