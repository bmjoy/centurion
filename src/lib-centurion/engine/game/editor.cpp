#include "editor.h"

#include "interface/editorUi.h"

#include <game/strategy.h>
#include <picking.h>
#include <engine.h>
#include <menu/menu.h>
#include <logger.h>
#include <data.h>
#include <settings.h>
#include <translationsTable.h>

#include <gl_terrain.h>
#include <gl_items.h>
#include <game/interface/editorMenuBar.h>
#include <game/interface/editorWindows.h>

#include <hector-lua.h>
#include <hector-misc.h>

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
			bool clickedOnUIElement = false;
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
				Editor::InsertingObject();

				Game::RenderObjects();
				SelectionRectangle::Render();

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
			try
			{
				// this function IS RUN EVERY FRAME (see Editor::Run)
				// This if is true when information is passed by LUA
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
					Editor::tmpObject->MarkAsMoving();
					EditorWindows::Hide();
					EditorMenuBar::Hide();
					return;
				}

				// this if is the one that is run every frame!!
				if (Editor::tmpObject == nullptr)
				{
					return;
				}

				// if i'm here it means that i selected to insert an object but i want to abort the operation
				if (Engine::Mouse::RightClick || Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
				{
					delete Editor::tmpObject;
					Editor::tmpObject = nullptr;
					EditorMenuBar::Show();
					EditorWindows::Show();
					EditorUI::UpdateInfoText(L"");
					Engine::Mouse::RightClick = false;
					Engine::Keyboard::SetKeyStatus(GLFW_KEY_ESCAPE, 0);
					return;
				}

				// if i'm here it means that i selected to insert an object and i want to place and create it
				if (Engine::Mouse::LeftClick)
				{
					if (Editor::tmpObject->IsPlaceable() == true)
					{
						if (Game::CreateObject(tmpObject->GetClassName(), tmpObject->GetPosition().x, tmpObject->GetPosition().y, 1) == nullptr) throw;
						Minimap::Update();
						Engine::Mouse::LeftClick = false;
						Game::Map::MarkAsEdited();
						return;
					}
				}

				// if i'm here it means that i'm not placing or aborting and i'm choosing the object position
				Editor::tmpObject->SetPosition(vec3(Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate(), 0.f));
				Editor::tmpObject->Render(false, 0);
				Editor::tmpObject->SendInfoText(OBJ_INFOTEXT_INSERTING);
			}
			catch (...)
			{
				throw;
			}

		}

		void Game::Editor::ShiftSelectedObject(void)
		{
			// this function IS RUN EVERY FRAME (see Editor::Run)

			// if no object is selected --> exit
			GObject* obj = Game::GetSelectedObject();
			if (obj == nullptr) return;

			if (Engine::Mouse::LeftClick)
			{
				Editor::movingObject.clickedOnUIElement = (Picking::UI::GetLeftClickId() != 0);
			}

			// all the exceptions
			if (EditorWindows::IsThereAnyWindowOpen() == true) return;
			if (Editor::movingObject.clickedOnUIElement) return;

			// if an object is selected --> 
			if (Engine::Mouse::LeftHold == false)
			{
				obj->MarkAsNotMoving();

				// if i'm releasing the left hold and i want to place the object
				if (Editor::movingObject.isActive == true)
				{
					// check placeability
					if (obj->IsPlaceable() == true)
					{
						obj->UpdatePass();
						Minimap::Update();
						Game::Map::MarkAsEdited();
					}
					else
					{
						obj->SetPosition(vec3(Editor::movingObject.ObjectXPos, Editor::movingObject.ObjectYPos, 0.f));
						obj->UpdatePass();
					}

					// reset values
					EditorUI::UpdateInfoText(L"");
					EditorWindows::Show();
					EditorMenuBar::Show();
					Editor::movingObject.isActive = false;
				}

				// every frame i save information about the object beginning position
				Editor::movingObject.ObjectXPos = obj->GetPosition().x;
				Editor::movingObject.ObjectYPos = obj->GetPosition().y;
				Editor::movingObject.StartXMouse = Engine::Mouse::GetXMapCoordinate();
				Editor::movingObject.StartYMouse = Engine::Mouse::GetYMapCoordinate();

				return;
			}

			// i'm beginning to move the object, i clear the pass and hide things
			if (Editor::movingObject.isActive == false)
			{
				obj->MarkAsMoving();
				obj->ClearPass();
				EditorWindows::Hide();
				EditorMenuBar::Hide();
				Editor::movingObject.isActive = true;
			}

			// new position calculation
			float dx = Engine::Mouse::GetXMapCoordinate() - Editor::movingObject.StartXMouse;
			float dy = Engine::Mouse::GetYMapCoordinate() - Editor::movingObject.StartYMouse;
			obj->SetPosition(vec3(Editor::movingObject.ObjectXPos + dx, Editor::movingObject.ObjectYPos + dy, 0.f));
			obj->SendInfoText(OBJ_INFOTEXT_MOVING);
		}

		void GenerateRandomMap(const int nPlayers)
		{
			Game::Map::CreateNoise();
			Game::Map::MarkAsEdited();
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
			if (Minimap::IsActive() == true) return;
			if (type.empty() == false)
			{
				changingTerrain.isActive = true;
				Mapgen::TerrainTexture* tt = Mapgen::GetTerrainTexturePtrByName(type);
				if (tt == nullptr) {
					changingTerrain.isActive = false;
					changingTerrain.type = -1.f;
					return;
				}

				// This part of code is executed when you are BEGINNING the terrain change
				changingTerrain.type = (float)tt->GetId();
				EditorWindows::Hide();
				std::wstring infoText = TranslationsTable::GetWTranslation(Engine::Data::GetWordFromDictionaryById(3));
				EditorUI::UpdateInfoText(infoText);
				Engine::Mouse::RightClick = false;
				return;
			}

			if (changingTerrain.type == -1.f) return;

			if (Engine::Mouse::RightClick == true || Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
			{
				// This part of code is executed when you are FINISHING the terrain change
				changingTerrain.isActive = false;
				changingTerrain.type = -1.f;
				EditorMenuBar::Show();
				EditorWindows::Show();
				EditorUI::UpdateInfoText(L"");
				Engine::Mouse::ChangeCursorType(CURSOR_TYPE_DEFAULT);
				Engine::Keyboard::SetKeyStatus(GLFW_KEY_ESCAPE, 0);
				return;
			}

			if (changingTerrain.isActive == false) return;

			// This part of code is executed when you are DOING the terrain change
			if (EditorMenuBar::IsHidden() == false) EditorMenuBar::Hide();
			if (Engine::Mouse::GetCursorType() != CURSOR_TYPE_CIRCLE) Engine::Mouse::ChangeCursorType(CURSOR_TYPE_CIRCLE);

			if (Engine::Mouse::LeftClick == false && Engine::Mouse::LeftHold == false) return;

			Game::Mapgen::ChangeTerrainTexture(changingTerrain.type, Engine::Mouse::GetXMapCoordinate(), Engine::Mouse::GetYMapCoordinate());
			Game::Map::MarkAsEdited();
			Minimap::Update();
		}

		bool IsChangingTerrain(void)
		{
			return changingTerrain.isActive;
		}

		void Game::Editor::Close(void)
		{
			if (Game::Map::IsMapEdited())
			{
				AskForSaving(); //QUESTA PARTE DI CODICE E LA RELATIVA FUNZIONE DEVONO ESSERE RIVISTI
			}
			else
			{
				Editor::isCreated = false;
				Engine::SetEnvironment("menu");
				Menu::Reset();
			}
		}

		#pragma region Popup Windows
		void AskForClosing(void)
		{
			Hector::EnableQuestionWindow();
			Hector::ExposeQuestionWindowText(TranslationsTable::GetTranslation("EDITOR_closingConfirmation"));
			Hector::SetQuestionWindowYesCmd("Editor.Close()");
			Hector::SetQuestionWindowNoCmd("");
		}

		void AskForSaving(void)
		{
			Hector::EnableQuestionWindow();
			Hector::ExposeQuestionWindowText(TranslationsTable::GetTranslation("EDITOR_saveBeforeNewOperation"));
			if (Game::Map::GetName() == "") {
				Hector::SetQuestionWindowYesCmd("OpenWindow(2)");
			}
			else
			{
				Hector::SetQuestionWindowYesCmd("Editor.SaveScenario()");
			}
			Hector::SetQuestionWindowNoCmd("");
		}

		void AskForSettlementDeleting(void)
		{
			if (Game::GetSelectedObject() == nullptr) return;
			if (Game::GetSelectedObject()->AsBuilding()->GetSettlement() == nullptr) return;

			Hector::EnableQuestionWindow();
			Hector::ExposeQuestionWindowText(TranslationsTable::GetTranslation("EDITOR_deleteSettlement"));
			Hector::SetQuestionWindowYesCmd("Object.Remove(Selo():GetID())");
			Hector::SetQuestionWindowNoCmd("");
		}
		#pragma endregion

		void Game::Editor::handleKeyboardControls(void)
		{
			if (Hector::ConsoleIsActive()) return;
			if (Editor::IsMovingObject()) return;
			if (Editor::IsInsertingObject()) return;

			if (Editor::IsChangingTerrain() == false)
			{
				if (EditorWindows::AnyWindowIsOpened() == false) {
					if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
					{
						GObject::ResetGameObjects();
						Close();
						return;
					}
				}

				if (EditorWindows::AnyWindowIsOpened() == true)
				{
					if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE))
					{
						EditorWindows::CloseEveryWindow();
					}
					return;
				}
			}

			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_DELETE))
			{
				if (Game::IsGameObjectNotNull(Picking::Obj::GetLeftClickId()))
				{
					GObject* b = Game::GetSelectedObject();
					if (b != nullptr && b->IsBuilding() == true)
					{
						if (b->AsBuilding()->IsCentralBuilding())
						{
							if (b->AsBuilding()->GetSettlement()->GetBuildingsBelongToSettlement().size() > 1)
							{
								AskForSettlementDeleting();
							}
							else
							{
								GObject::RemoveGameObject(b->GetPickingID());
							}
						}
						else
						{
							GObject::RemoveGameObject(b->GetPickingID());
						}
					}
				}
			}

			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_SPACE) || Engine::Mouse::MiddleClick)
			{
				if (Minimap::IsActive()) {
					Minimap::Disable();
					SelectionRectangle::ResetCoordinates();
				}
				else {
					Minimap::Enable();
					if (Engine::Mouse::GetCursorType() != CURSOR_TYPE_DEFAULT) Engine::Mouse::ChangeCursorType(CURSOR_TYPE_DEFAULT);
				}
				Minimap::IsActive() ? EditorMenuBar::Hide() : EditorMenuBar::Show();
				Minimap::IsActive() ? Logger::Info("Minimap ON!") : Logger::Info("Minimap OFF!");
			}
		}
	};
};