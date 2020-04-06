#include "game.h"

#include "editor.h"

#include <picking.h>
#include <engine.h>
#include <classes/building.h>
#include <classes/decoration.h>
#include <player/player.h>
#include <game/interface/editorWindows.h>

using namespace std;
using namespace glm;

#pragma region Extern variables

glm::mat4 Game::projectionMatrix;
glm::mat4 Game::viewMatrix;
bool Game::isCreated = false;
float Game::cameraToX;
float Game::cameraToY;
int Game::numberOfPlayers = 1;
vector<string> Game::racesName;
map<string, Game::Race> Game::races;
vector<vec3> Game::listOfColors;
GObject* Game::selectedObject = nullptr;

#pragma endregion



bool Game::IsCreated()
{
	return isCreated;
}

void Game::SetNumberOfPlayers(int n)
{
	numberOfPlayers = n;
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
	Game::selectedObject = o;
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
		cameraToX = Engine::Mouse::GetXLeftClick() / Engine::myWindow::Width*(float)MEDIUM_MAP_WIDTH - Engine::myWindow::WidthZoomed / 2.f;
		cameraToY = Engine::Camera::GetYMinimapCoordinate(Engine::Mouse::GetYLeftClick()) / Engine::myWindow::Height*(float)MEDIUM_MAP_HEIGHT - Engine::myWindow::HeightZoomed / 2.f;
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

Game::Race *Game::GetRace(string race_name)
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

void Game::GenerateOutposts(vector<vec2> &locs) {

	string className = "routpost";

	/*for (int i = 0; i < locs.size(); i++) {
		Building* b = new Building();
		b->SetClassName(className);
		b->SetType("building");
		b->SetPlayer(0);
		b->SetPosition(vec3(locs[i].x, locs[i].y, 0.f));
		b->SetPickingID(Picking::Obj::ObtainPickingID());
		b->GetSettlement().SetSettlementName("Outpost_" + i);
		b->create();
		GameObjects[b->GetPickingID()] = b;
	}*/
}

/*
vector<Building*> Game::GetListOfStandAloneBuildings()
{
	vector<Building*> saBuildings = vector<Building*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GObject::GetObjectByID(i) != nullptr && GObject::GetObjectByID(i)->IsBuilding()) {
			if (GObject::GetObjectByID(i)->AsBuilding()->get_settlement_building() == nullptr && GObject::GetObjectByID(i)->AsBuilding()->GetSettlement().IsIndipendent() == false) {
				saBuildings.push_back(GObject::GetObjectByID(i)->AsBuilding());
			}
		}
	}
	return saBuildings;
}
*/

#pragma region Map Generation Methods

void Game::GenerateSettlements(vector<vec2> &locs) {
	//string RandomMapSettlementPath = "scenarios/RandomMapSettlements/";

	//vector<int> TemporaryMapTextures;

	//int num_townhalls = (int)locs.size();

	//// read texture
	//{
	//	fstream fin;
	//	fin.open(RandomMapSettlementPath + "texture");
	//	string line, number;
	//	getline(fin, line);
	//	stringstream s(line);
	//	int i = 0;
	//	while (getline(s, number, ',')) {
	//		TemporaryMapTextures.push_back(stoi(number));
	//		i++;
	//	}
	//}

	//for (int i = 0; i < num_townhalls; i++) {
	//	string r = playersList[i].GetPlayerRace().substr(5);
	//	vec2 origin = locs[i];
	//	playersList[i].SetStartPoint(origin);
	//	string SettlementRace = "Settlement_" + r + "_1";
	//	fstream fin;
	//	fin.open(RandomMapSettlementPath + "objects.tsv");
	//	string line, value;
	//	int row = 0;
	//	while (getline(fin, line)) {
	//		if (row > 0) {
	//			string objectsInfo[9];
	//			stringstream s(line);
	//			int v = 0;
	//			while (getline(s, value, '\t')) { objectsInfo[v] = value; v++; }
	//			string settl_name = objectsInfo[3];
	//			if (settl_name == SettlementRace) {
	//				string type = objectsInfo[0];
	//				string className = objectsInfo[1];
	//				string settl_name = objectsInfo[3];
	//				float xOffset = stof(objectsInfo[7]);
	//				float yOffset = stof(objectsInfo[8]);

	//				float xEditorPos = stof(objectsInfo[5]);
	//				float yEditorPos = stof(objectsInfo[6]);

	//				if (type == "building") {
						//Building* b = new Building();
						//b->SetClassName(className);
						//b->SetType(type);
						//b->SetPlayer(i);
						//b->SetPosition(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						//b->SetPickingID(Picking::Obj::ObtainPickingID());
						//b->GetSettlement().SetSettlementName("Settlement_player_" + i);
						//b->create();
						//GameObjects[b->GetPickingID()] = b;

						//if (b->GetSettlement().IsIndipendent()) {
						//	// update terrain around the townhall
						//	// N.B: mapgen::grid_size * 2 because the map has "borders"

						//	int EditorStartPointX = (int)xEditorPos - TOWNHALL_RADIUS + mapgen::grid_size * 2;
						//	int EditorStartPointY = (int)yEditorPos - TOWNHALL_RADIUS + mapgen::grid_size * 2;
						//	int NewMapStartPointX = (int)origin.x - TOWNHALL_RADIUS + mapgen::grid_size * 2;
						//	int NewMapStartPointY = (int)origin.y - TOWNHALL_RADIUS + mapgen::grid_size * 2;

						//	for (int iHoriz = 0; iHoriz <= TOWNHALL_RADIUS * 2; iHoriz += mapgen::grid_size) {
						//		for (int iVert = 0; iVert <= TOWNHALL_RADIUS * 2; iVert += mapgen::grid_size) {

						//			int EditorPointX = EditorStartPointX + iHoriz;
						//			int EditorPointY = EditorStartPointY + iVert;
						//			int NewMapPointX = NewMapStartPointX + iHoriz;
						//			int NewMapPointY = NewMapStartPointY + iVert;

						//			if (math::euclidean_distance((float)NewMapPointX, (float)NewMapPointY, origin.x + mapgen::grid_size * 2.f, origin.y + mapgen::grid_size * 2.f) > TOWNHALL_RADIUS) continue;

						//			int EditorPointLoc = mapgen::getVertexPos(EditorPointX, EditorPointY);
						//			int NewMapPointLoc = mapgen::getVertexPos(NewMapPointX, NewMapPointY);
						//			mapgen::MapTextures()[NewMapPointLoc] = float(TemporaryMapTextures[EditorPointLoc]);
						//		}
						//	}
						//}
					//}
					//if (type == "decoration") {
						/*Decoration* d = new Decoration();
						d->SetClassName(className);
						d->SetPlayer(0);
						d->SetPosition(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						d->SetPickingID(Picking::Obj::ObtainPickingID());
						d->create();
						GameObjects[d->GetPickingID()] = d;*/
					//}
				//}
			//}
			//row++;
		//}
	//}

	// update texture buffer
	//MapTerrain()->updateTextureBuffer();

	// update buildings info
	//UpdateSettlementBuildings();
}

//void Game::UpdateSettlementBuildings() {
	/*vector<Building*> listOfBuildings = Game::GetListOfBuildings();
	vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();

	for (int i = 0; i < listOfBuildings.size(); i++) {
		Building* bld = listOfBuildings[i];
		if (!bld->GetSettlement().IsIndipendent()) {
			for (int j = 0; j < listOfIndipBuildings.size(); j++) {
				Building* settl = listOfIndipBuildings[j];
				if (settl->GetSettlement().GetSettlementName() == bld->GetSettlement().GetSettlementName()) {
					bld->set_settlement_building(settl);
					break;
				}
			}
		}
	}*/
//}

#pragma endregion


#pragma region TO-LUA METHODS

GObject* Game::GetSelectedObject(void)
{
	return Game::selectedObject;
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
		ObjectData::ObjectXMLClassData *objData = ObjectData::GetObjectData(className);
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

#pragma endregion

