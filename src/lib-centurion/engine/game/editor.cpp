#include "editor.h"

#include "interface/editorUi.h"

#include <game/strategy.h>
#include <picking.h>
#include <engine.h>
#include <menu/menu.h>
#include <logger.h>
#include <settings.h>
#include <translationsTable.h>

#include <gl_terrain.h>
#include <gl_items.h>
#include <game/interface/editorMenuBar.h>
#include <game/interface/editorWindows.h>

#include <hector-lua.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace Game
{
	namespace Editor 
	{
		struct MovingObject
		{
			bool isActive = false;
			float ObjectXPos = 0.f;
			float ObjectYPos = 0.f;
			float StartXMouse = 0.f;
			float StartYMouse = 0.f;
		};

		struct ChangingTerrain
		{
			bool isActive = false;
			float type = -1.f;
		};

		// Private variables
		namespace
		{
			std::vector<std::array<std::string, 3>> editorObjectTree = std::vector<std::array<std::string, 3>>();
			std::vector<std::string> editorObjectTreeList1 = std::vector<std::string>();
			std::vector<std::string> editorObjectTreeList2 = std::vector<std::string>();
			std::vector<std::string> editorObjectTreeList3 = std::vector<std::string>();

			std::vector<std::array<std::string, 2>> editorTerrainTree = std::vector<std::array<std::string, 2>>();
			std::vector<std::string> editorTerrainTreeList1 = std::vector<std::string>();
			std::vector<std::string> editorTerrainTreeList2 = std::vector<std::string>();

			GObject* tmpObject = nullptr;

			MovingObject movingObject = MovingObject();
			ChangingTerrain changingTerrain = ChangingTerrain();

			bool isCreated = false;
		}

		void Game::Editor::Create(void)
		{
			Picking::UI::ResetPicking();
			Picking::Obj::ResetPicking();

			Game::ResetGame();
			Map::Reset();
			Engine::myWindow::BottomBarHeight = 0.f;
			Engine::myWindow::TopBarHeight = 0.f;

			GLItems::setMinimapProjectionMatrix();
			
			EditorUI::Create();

			SelectionRectangle::Create();

			Engine::Mouse::LeftClick = false;
			Engine::Mouse::RightClick = false;
			Engine::Mouse::LeftHold = false;

			Editor::tmpObject = nullptr;

			Engine::Camera::GoToPoint(1.f, 1.f);

			Editor::isCreated = true;
			Editor::movingObject.isActive = false;
			Editor::changingTerrain.isActive = false;
			Minimap::Update();
		}

		void Game::Editor::Run(void)
		{
			if (Editor::isCreated == false)
			{
				//Audio()->MusicStop();
				Editor::Create();
				Logger::Info("Editor has been created!");
			}

			Picking::UI::ResetClickIds();

			/* Keyboard control */
			handleKeyboardControls();

			/* If minimap is NOT active */
			if (!Minimap::IsActive())
			{
				Engine::Camera::mouseControl();
				Engine::Camera::keyboardControl();

				viewMatrix = Engine::Camera::calculateViewMatrix();
				projectionMatrix = GLItems::getCameraProjectionMatrix();

				// PICKING UI RENDERING
				EditorUI::Render(true);
				Picking::UI::UpdateClickIds();

				// apply game matrices
				GLItems::applyGameMatrices(&projectionMatrix, &viewMatrix);

				// PICKING OBJECTS RENDERING
				RenderObjectsPicking();

				// NORMAL RENDERING
				Map::Render(false);
				Editor::ChangeTerrainType();
				Editor::ShiftSelectedObject();
				Game::RenderObjects();
				Editor::InsertingObject();

				// apply menu matrices
				GLItems::applyMenuMatrices();

				EditorUI::Render(false);
			}

			/* If minimap is active */
			else
			{
				viewMatrix = mat4(1.0f);
				projectionMatrix = GLItems::getMinimapProjectionMatrix();

				// PICKING RENDERING
				EditorUI::Render(true);
				Picking::UI::UpdateClickIds();

				// apply game matrices
				GLItems::applyGameMatrices(&projectionMatrix, &viewMatrix);

				// NORMAL RENDERING 
				Minimap::RenderMapAndObjects();
				Map::Render(false);
				Picking::Obj::UpdateClickIds();

				// apply menu matrices
				GLItems::applyMenuMatrices();

				Minimap::RenderRectangle();
				EditorUI::Render(false);

				if (Picking::UI::GetLeftClickId() == 0) GoToPointFromMinimap();
			}

			GLItems::setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));
		}

		void Game::Editor::AddEditorObjectTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3)
		{
			std::array<std::string, 3> element = { filter1, filter2, filter3 };
			Editor::editorObjectTree.push_back(element);
		}

		std::vector<std::string>* Game::Editor::GetEditorObjectTreeList1(void)
		{
			Editor::editorObjectTreeList1.clear();
			for (auto i : Editor::editorObjectTree)
			{
				if (std::find(Editor::editorObjectTreeList1.begin(), Editor::editorObjectTreeList1.end(), i[0]) != Editor::editorObjectTreeList1.end())
					continue;
				Editor::editorObjectTreeList1.push_back(i[0]);
			}
			std::sort(Editor::editorObjectTreeList1.begin(), Editor::editorObjectTreeList1.end());
			return &Editor::editorObjectTreeList1;
		}

		std::vector<std::string>* Game::Editor::GetEditorObjectTreeList2(const std::string filter1)
		{
			Editor::editorObjectTreeList2.clear();
			for (auto i : Editor::editorObjectTree)
			{
				if (std::find(Editor::editorObjectTreeList2.begin(), Editor::editorObjectTreeList2.end(), i[1]) != Editor::editorObjectTreeList2.end())
					continue;
				if (i[0] != filter1) continue;
				Editor::editorObjectTreeList2.push_back(i[1]);
			}
			std::sort(Editor::editorObjectTreeList2.begin(), Editor::editorObjectTreeList2.end());
			return &Editor::editorObjectTreeList2;
		}

		std::vector<std::string>* Game::Editor::GetEditorObjectTreeList3(const std::string filter1, const std::string filter2)
		{
			Editor::editorObjectTreeList3.clear();
			for (auto i : Editor::editorObjectTree)
			{
				if (std::find(Editor::editorObjectTreeList3.begin(), Editor::editorObjectTreeList3.end(), i[1]) != Editor::editorObjectTreeList3.end())
					continue;
				if (i[0] != filter1 || i[1] != filter2) 
					continue;
				Editor::editorObjectTreeList3.push_back(i[2]);
			}
			std::sort(Editor::editorObjectTreeList3.begin(), Editor::editorObjectTreeList3.end());
			return &Editor::editorObjectTreeList3;
		}

		void AddEditorTerrainTreeElement(const std::string& filter1, const std::string& filter2)
		{
			std::array<std::string, 2> element = { filter1, filter2 };
			Editor::editorTerrainTree.push_back(element);
		}

		std::vector<std::string>* GetEditorTerrainTreeList1(void)
		{
			Editor::editorTerrainTreeList1.clear();
			for (auto i : Editor::editorTerrainTree)
			{
				if (std::find(Editor::editorTerrainTreeList1.begin(), Editor::editorTerrainTreeList1.end(), i[0]) != Editor::editorTerrainTreeList1.end())
					continue;
				Editor::editorTerrainTreeList1.push_back(i[0]);
			}
			std::sort(Editor::editorTerrainTreeList1.begin(), Editor::editorTerrainTreeList1.end());
			return &Editor::editorTerrainTreeList1;
		}

		std::vector<std::string>* GetEditorTerrainTreeList2(const std::string filter1)
		{
			Editor::editorTerrainTreeList2.clear();
			for (auto i : Editor::editorTerrainTree)
			{
				if (std::find(Editor::editorTerrainTreeList2.begin(), Editor::editorTerrainTreeList2.end(), i[1]) != Editor::editorTerrainTreeList2.end())
					continue;
				if (i[0] != filter1) continue;
				Editor::editorTerrainTreeList2.push_back(i[1]);
			}
			std::sort(Editor::editorTerrainTreeList2.begin(), Editor::editorTerrainTreeList2.end());
			return &Editor::editorTerrainTreeList2;
		}

		void Game::Editor::InsertingObject(std::string type, std::string className)
		{
			if (type.empty() == false && className.empty() == false)
			{
				if (type == "buildings")
				{
					Editor::tmpObject = new Building();
				}
				else if (type == "units")
				{
					//Editor::tmpObject = new Unit();
				}
				else if (type == "decorations")
				{
					Editor::tmpObject = new Decoration();
				}
				else
					return;

				Editor::tmpObject->Create(className, true);
				Editor::tmpObject->SetPlayer(1);
				EditorMenuBar::Hide();
				EditorWindows::Hide();
				return;
			}

			if (Editor::tmpObject == nullptr) 
			{
				EditorMenuBar::Show();
				EditorWindows::Show();
				EditorUI::UpdateInfoText("");
				return;
			}
			
			if (Engine::Mouse::RightClick)
			{
				delete Editor::tmpObject;
				Editor::tmpObject = nullptr;
				EditorMenuBar::Show();
				EditorWindows::Show();
				EditorUI::UpdateInfoText("");
				Engine::Mouse::RightClick = false;
			}

			if (Engine::Mouse::LeftClick)
			{
				if (Editor::tmpObject->IsPlaceable() == true)
				{
					Game::CreateObject(tmpObject->GetClassName(), tmpObject->GetPosition().x, tmpObject->GetPosition().y, 1);
					delete Editor::tmpObject;
					Editor::tmpObject = nullptr;
					EditorMenuBar::Show();
					EditorWindows::Show();
					Engine::Mouse::LeftClick = false;
				}
			}

			if (Editor::tmpObject == nullptr) return;
			if (Editor::tmpObject->IsBeingMoved() == false) Editor::tmpObject->MarkAsMoving();
			Editor::tmpObject->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 0.f));
			Editor::tmpObject->Render(false, 0);
		}

		void Game::Editor::ShiftSelectedObject(void)
		{
			GObject *obj = Game::GetSelectedObject();
			if (obj == nullptr) return;

			if (EditorWindows::IsThereAnyWindowOpen() == true)
				return;

			if (Engine::Mouse::LeftHold == false)
			{
				obj->MarkAsNotMoving();
				if (Editor::movingObject.isActive == true)
				{
					if (obj->IsPlaceable() == true)
					{
						obj->UpdatePass();
					}
					else
					{
						obj->SetPosition(vec3(Editor::movingObject.ObjectXPos, Editor::movingObject.ObjectYPos, 0.f));
						obj->UpdatePass();
					}
					Editor::movingObject.isActive = false;
				}
				Editor::movingObject.ObjectXPos = obj->GetPosition().x;
				Editor::movingObject.ObjectYPos = obj->GetPosition().y;
				Editor::movingObject.StartXMouse = Engine::Mouse::GetXMapCoordinate();
				Editor::movingObject.StartYMouse = Engine::Mouse::GetYMapCoordinate();
				return;
			}

			if (Editor::movingObject.isActive == false)
			{
				obj->MarkAsMoving();
				obj->ClearPass();
				obj->UpdatePass();
				Editor::movingObject.isActive = true;
			}

			float dx = Engine::Mouse::GetXMapCoordinate() - Editor::movingObject.StartXMouse;
			float dy = Engine::Mouse::GetYMapCoordinate() - Editor::movingObject.StartYMouse;
			obj->SetPosition(vec3(Editor::movingObject.ObjectXPos + dx, Editor::movingObject.ObjectYPos + dy, 0.f));
		}

		bool Game::Editor::IsInsertingObject(void)
		{
			return (Editor::tmpObject != nullptr);
		}

		bool Game::Editor::IsMovingObject(void)
		{
			return Editor::movingObject.isActive;
		}

		void ChangeTerrainType(std::string type)
		{
			if (type.empty() == false) 
			{
				changingTerrain.isActive = true;
				Mapgen::TerrainTexture * tt = Mapgen::GetTerrainTexturePtrByName(type);
				if (tt == nullptr) {
					changingTerrain.isActive = false;
					changingTerrain.type = -1.f;
					return;
				}

				// This part of code is executed when you are BEGINNING the terrain change
				Game::Minimap::Update();
				changingTerrain.type = (float)tt->GetId();
				EditorMenuBar::Hide();
				EditorWindows::Hide();
				EditorUI::UpdateInfoText(TranslationsTable::GetTranslation("EDITOR_cancelBrush"));
				Engine::Mouse::ChangeCursorType(CURSOR_TYPE_CIRCLE);
				return;
			}

			if (changingTerrain.type == -1.f) 
				return;
			if (Engine::Mouse::RightClick == true)
			{
				// This part of code is executed when you are FINISHING the terrain change
				changingTerrain.isActive = false;
				EditorMenuBar::Show();
				EditorWindows::Show();
				EditorUI::UpdateInfoText("");
				Engine::Mouse::ChangeCursorType(CURSOR_TYPE_DEFAULT);
				return;
			}

			if (changingTerrain.isActive == false) return;

			if (Engine::Mouse::LeftClick == false && Engine::Mouse::LeftHold == false) return;

			// This part of code is executed when you are DOING the terrain change

			float xPos = Engine::Mouse::GetXMapCoordinate();
			float yPos = Engine::Mouse::GetYMapCoordinate();
			

			int x = int(round(xPos / Game::Mapgen::GetTriangleWidth())) * Game::Mapgen::GetTriangleWidth() + Game::Mapgen::GetTriangleWidth() * 2;
			int y = int(round(yPos / Game::Mapgen::GetTriangleWidth())) * Game::Mapgen::GetTriangleWidth() + Game::Mapgen::GetTriangleWidth() * 2;

			int j = Game::Mapgen::getVertexPos(x, y);

			if (Game::Mapgen::MapTextures()[j] != changingTerrain.type) 
			{
				Game::Mapgen::MapTextures()[j] = changingTerrain.type;
				GLItems::MapTerrain()->updateTextureBuffer();
			}
		}

		bool IsChangingTerrain(void)
		{
			return changingTerrain.isActive;
		}

		void Game::Editor::ToggleGrid(void)
		{
			if (Map::IsGridEnabled()) Map::DisableGrid();
			else Map::EnableGrid();
			Map::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
		}

		void Game::Editor::Close(void)
		{
			Editor::isCreated = false;
			Engine::SetEnvironment("menu");
			Menu::Reset();

		}

		void Game::Editor::handleKeyboardControls(void)
		{
			if (Hector::ConsoleIsActive()) return;

			if (EditorWindows::AnyWindowIsOpened() == false) {
				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
				{
					GObject::ResetGameObjects();
					Close();
					return;
				}
			}

			if (EditorWindows::AnyWindowIsOpened() == true) {

				if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
				{
					EditorWindows::CloseEveryWindow();
				}
				return;
			}



			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Engine::Keyboard::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL))
			{
				if (Minimap::IsActive() == false)
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
				if (Game::IsGameObjectNotNull(Picking::Obj::GetLeftClickId()))
				{
					GObject* b = Game::GetSelectedObject();
					if (b != nullptr && b->IsBuilding() == true)
					{
						GObject::RemoveGameObject(b->GetPickingID());
					}
				}
			}

			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_SPACE) || Engine::Mouse::MiddleClick)
			{
				if (Minimap::IsActive()) Minimap::Disable();
				else Minimap::Enable();
				Minimap::IsActive() ? EditorMenuBar::Hide() : EditorMenuBar::Show();
				Minimap::IsActive() ? Logger::Info("Minimap ON!") : Logger::Info("Minimap OFF!");
			}


			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_Z))
			{
				Map::Wireframe = !Map::Wireframe;
				Map::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
			}

			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			{
				GObject::ResetGameObjects();
				Close();
			}
		}

	};
};

