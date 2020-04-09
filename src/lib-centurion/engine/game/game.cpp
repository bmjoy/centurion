#include "game.h"

#include "editor.h"

#include <picking.h>
#include <engine.h>
#include <classes/building.h>
#include <classes/decoration.h>
#include <player/player.h>
#include <game/interface/editorWindows.h>
#include <game/interface/editorMenuBar.h>



using namespace std;
using namespace glm;

#pragma region Extern variables
glm::mat4 Game::projectionMatrix;
glm::mat4 Game::viewMatrix;
vector<vec3> Game::listOfColors;
#pragma endregion



namespace Game
{
	namespace
	{
		int NUMBER_OF_PLAYERS = 1;
		GObject* SELO = nullptr;

		float cameraToX = 0.f, cameraToY = 0.f;

		// Provvisorio!!
		vector<string> racesName;
		map<string, Game::Race> races;
	};


	void Game::SetNumberOfPlayers(int n)
	{
		NUMBER_OF_PLAYERS = n;
	}

	void Game::ResetGame()
	{
		GObject::ResetGameObjects();
		Map::DisableGrid();
		Minimap::Unblock();
		Minimap::Disable();
	}

	void Game::SetSelectedObject(GObject* o)
	{
		Game::SELO = o;
	}

	vector<Building*> Game::GetListOfIndipendentBuildings(void)
	{
		vector<Building*> indipBuildings = vector<Building*>();
		for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (GObject::GetObjectByID(i) != nullptr && GObject::GetObjectByID(i)->IsBuilding())
			{
				if (GObject::GetObjectByID(i)->AsBuilding()->GetSettlement()->IsIndipendent())
				{
					indipBuildings.push_back(GObject::GetObjectByID(i)->AsBuilding());
				}
			}
		}
		return indipBuildings;
	}

	void Game::RenderObjectsPicking() {

		// Various exceptions
		if (EditorWindows::AnyWindowIsOpened()) return;
		if (Editor::IsInsertingObject()) return;
		if (Editor::IsMovingObject()) return;
		if (SelectionRectangle::IsActive()) return;
		if (!Engine::Mouse::RightClick && !Engine::Mouse::LeftClick) return;

		if (Picking::UI::GetLeftClickId() != 0) {
			//Picking::Obj::ResetClickIds();
			return;
		}

		for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
			if (GObject::GetObjectByID(i) != nullptr) {
				GObject::GetObjectByID(i)->Render(true);
			}
		}

		Picking::Obj::UpdateClickIds();
	}

	void Game::RenderObjects()
	{
		for (unsigned int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++)
		{
			if (GObject::GetObjectByID(i) != nullptr)
			{
				GObject::GetObjectByID(i)->Render(false, Picking::Obj::GetLeftClickId());
			}
		}

		if (!Minimap::IsActive() && Picking::UI::GetLeftClickId() == 0)
			SelectionRectangle::Render(); //&& !editor::movingObject
	}

	void Game::GoToPointFromMinimap() {
		if (Engine::Mouse::LeftClick && Engine::Mouse::IsCursorInGameScreen()) {
			cameraToX = Engine::Mouse::GetXLeftClick() / Engine::myWindow::Width * (float)MEDIUM_MAP_WIDTH - Engine::myWindow::WidthZoomed / 2.f;
			cameraToY = Engine::Camera::GetYMinimapCoordinate(Engine::Mouse::GetYLeftClick()) / Engine::myWindow::Height * (float)MEDIUM_MAP_HEIGHT - Engine::myWindow::HeightZoomed / 2.f;
			// if you are clicking on a townhall you have to double click 
			// to move the camera there and quit minimap
			if (Picking::Obj::GetLeftClickId() > 0 && Picking::HasDoubleClicked()) {
				cameraToX = GObject::GetObjectByID(Picking::Obj::GetLeftClickId())->AsBuilding()->get_xPos() - Engine::myWindow::WidthZoomed / 2.f;
				cameraToY = GObject::GetObjectByID(Picking::Obj::GetLeftClickId())->AsBuilding()->get_yPos() - Engine::myWindow::HeightZoomed / 2.f;
				Minimap::Unblock();
			}
			//------------------------------------------------
			if (Game::Minimap::IsBlocked() == false) {
				Engine::Camera::GoToPoint(cameraToX, cameraToY);
				Minimap::Disable();
				EditorMenuBar::Show();
				Engine::Mouse::LeftClick = false;
				Engine::Mouse::LeftHold = false;
			}
		}
	}

	void Game::AddRace(string race_name, Race r)
	{
		races[race_name] = r;
		racesName.push_back("RACE_" + race_name);
	}

	std::vector<std::string> Game::GetListOfRacesNames()
	{
		return racesName;
	}

	Game::Race* Game::GetRace(string race_name)
	{
		if (races.count(race_name) > 0) {
			return &races[race_name];
		}
		return nullptr;
	}

	void Game::AddColor(const vec3 col)
	{
		Game::listOfColors.push_back(col);
	}

	vector<vec3> Game::GetListOfColors(void)
	{
		return Game::listOfColors;
	}

	vec3 Game::GetColor(const unsigned int i)
	{
		return Game::listOfColors[i];
	}

	unsigned int Game::GetNumberOfColors(void)
	{
		return (unsigned int)Game::listOfColors.size();
	}

	GObject* Game::GetSelectedObject(void)
	{
		return Game::SELO;
	}

	bool Game::IsGameObjectSelected(const unsigned int id)
	{
		if (IsGameObjectNotNull(id) == false)
			return false;
		return GObject::GetObjectByID(id)->IsSelected();
	}

	bool Game::IsGameObjectNotNull(const unsigned int id)
	{
		if (id > MAX_NUMBER_OF_OBJECTS)
			return false;
		else
			return (GObject::GetObjectByID(id) != nullptr);
	}

	bool Game::CreateObject(const string className, const float x, const float y, const unsigned int player)
	{
		bool bObjectCreated = false;
		if (Engine::GetEnvironment() == MENU_ENV) return bObjectCreated;
		Game::Minimap::Update();

		if (player <= MAX_NUMBER_OF_PLAYERS)
		{
			ObjectData::ObjectXMLClassData* objData = ObjectData::GetObjectData(className);
			if (objData == nullptr)
			{
				return bObjectCreated;
			}

			string type = objData->GetClassType();

			if (type == "cpp_buildingclass")
			{
				Building* newBuilding = new Building();
				newBuilding->SetPlayer(player);
				bObjectCreated = newBuilding->Create(className);
				if (bObjectCreated == false)
				{
					delete newBuilding;
					newBuilding = nullptr;
				}
			}
			if (type == "cpp_decorationclass")
			{
				Decoration* newDecoration = new Decoration();
				bObjectCreated = newDecoration->Create(className);
				if (bObjectCreated == false)
				{
					delete newDecoration;
					newDecoration = nullptr;
				}
			}
		}
		return bObjectCreated;
	}
};


