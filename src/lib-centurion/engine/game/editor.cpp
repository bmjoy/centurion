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

		// Private variables
		namespace
		{
			std::vector<std::array<std::string, 3>> editorTree = std::vector<std::array<std::string, 3>>();
			std::vector<std::string> editorTreeList1 = std::vector<std::string>();
			std::vector<std::string> editorTreeList2 = std::vector<std::string>();
			std::vector<std::string> editorTreeList3 = std::vector<std::string>();

			GObject* tmpObject = nullptr;

			MovingObject movingObject = MovingObject();

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

		void Game::Editor::AddEditorTreeElement(const std::string& filter1, const std::string& filter2, const std::string& filter3)
		{
			std::array<std::string, 3> element = { filter1, filter2, filter3 };
			Editor::editorTree.push_back(element);
		}

		std::vector<std::string>* Game::Editor::GetEditorTreeList1(void)
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

		std::vector<std::string>* Game::Editor::GetEditorTreeList2(const std::string filter1)
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

		std::vector<std::string>* Game::Editor::GetEditorTreeList3(const std::string filter1, const std::string filter2)
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
					Logger::Info("Object cannot be created!");
					delete Editor::tmpObject;
					Editor::tmpObject = nullptr;
					return;
				}
				Editor::tmpObject->SetPlayer(1);
				EditorMenuBar::Hide();
				EditorWindows::Hide();
				return;
			}

			if (Editor::tmpObject == nullptr) return;
			EditorMenuBar::Show();
			EditorWindows::Show();
			if (Engine::Mouse::RightClick)
			{
				delete Editor::tmpObject;
				Editor::tmpObject = nullptr;
				EditorMenuBar::Show();
				EditorWindows::Show();
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
					//(???)Da sistermare! Rimangono i riferimenti ai settlement quando si esce dall'editor e poi si rientra.
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
					Building* b = GObject::GObject::GetObjectByID(Picking::Obj::GetLeftClickId())->AsBuilding();
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
								b->ClearPass();
								GObject::RemoveGameObject(Picking::Obj::GetLeftClickId());
							}
							*/
						}
						else {
							cout << "[DEBUG] Building " << b->GetSingularName() << " deleted!\n";
							b->ClearPass();
							GObject::RemoveGameObject(Picking::Obj::GetLeftClickId());
						}
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
				//(???)Da sistermare! Rimangono i riferimenti ai settlement quando si esce dall'editor e poi si rientra.
				GObject::ResetGameObjects();
				Close();
			}
		}

	};
};

