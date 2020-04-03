#include "game.h"
#include <mapgen/mapgen.h>

#include "strategy.h"
#include <picking.h>
#include <maths.h>
#include <engine.h>

#include "editor.h"

#include <terrain.h>
#include <grid.h>
#include <gl_rectangle.h>
#include <minimap_rectangle.h>

#include <logger.h>
#include <settings.h>
#include <classes/object-data.h>
#include <file_manager.h>

using namespace std;
using namespace glm;

#pragma region Map class

bool Game::Map::Wireframe = false;
bool Game::Map::isGridEnabled;

Game::Map::Map(void) {}

void Game::Map::LoadScenario(const string scenarioName)
{
	try
	{
		string scenarioPath = "scenarios/" + scenarioName;

		LoadHeights(scenarioPath + "/heights");
		LoadTexture(scenarioPath + "/texture");
		LoadMapObjectsFromXml(scenarioPath + "/mapObjects.xml");

		//UpdateSettlementBuildings();
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred loading the following scenario: \"" + scenarioName + "\"", "", "Game::Map", "LoadScenario");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void Game::Map::SaveScenario(const string scenarioName)
{
	try
	{
		string scenarioPath = Folders::SCENARIOS + scenarioName;

		if (FileManager::CheckIfFolderExists(scenarioPath) == false) 
		{
			FileManager::CreateFolder(scenarioPath);
		}

		SaveHeights(scenarioPath + "/heights");
		SaveTexture(scenarioPath + "/texture");
		SaveMapObjectsToXml(scenarioPath + "/mapObjects.xml");


		Logger::LogMessage msg = Logger::LogMessage("The scenario is saved with the following name: \"" + scenarioName + "\"", "", "Game::Map", "SaveScenario");
		Logger::Info(msg);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred creating the following scenario: \"" + scenarioName + "\"", "", "Game::Map", "SaveScenario");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void Game::Map::SaveMapObjectsToXml(const string xmlPath)
{
	try
	{
		// save buildings, settlements, decorations, units
	}
	catch (...)
	{
		Engine::GameClose();
	}
}

void Game::Map::SaveHeights(const string path)
{
	try
	{
		ofstream heightsFile(path);
		if (heightsFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices * 4; i += 4)
				if (i == 0) {
					heightsFile << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
				else {
					heightsFile << "," << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
				}
		}
		heightsFile.close();
	}
	catch (...)
	{
		Engine::GameClose();
	}

}

void Game::Map::SaveTexture(const string path)
{
	try
	{
		ofstream textureFile(path);
		if (textureFile.is_open()) {
			for (int i = 0; i < mapgen::nVertices; i++)
				if (i == 0) {
					textureFile << mapgen::MapTextures()[i];
				}
				else {
					textureFile << "," << mapgen::MapTextures()[i];
				}
		}
		textureFile.close();
	}
	catch (...)
	{
		Engine::GameClose();
	}
}

void Game::Map::LoadMapObjectsFromXml(const string xmlPath)
{
	try
	{
		// load buildings, settlements, decorations, units
	}
	catch (...)
	{
		Engine::GameClose();
	}
}



void Game::Map::LoadHeights(const string path)
{
	try
	{
		fstream fin;
		fin.open(path);
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			mapgen::MapHeights()[i] = stof(number);
			i++;
		}
	}
	catch (...)
	{
		Engine::GameClose();
	}
}

void Game::Map::LoadTexture(const string path)
{
	try
	{
		fstream fin;
		fin.open(path);
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			mapgen::MapTextures()[i] = stof(number);
			i++;
		}
	}
	catch (...)
	{
		Engine::GameClose();
	}
}

bool Game::Map::IsGridEnabled(void)
{
	return Game::Map::isGridEnabled;
}

void Game::Map::EnableGrid(void)
{
	Game::Map::isGridEnabled = true;
}

void Game::Map::DisableGrid(void)
{
	Game::Map::isGridEnabled = false;
}

void Game::Map::Reset(void) 
{
	isGridEnabled = false;
	mapgen::reset_map();
	MapGrid()->reset();
	MapTerrain()->updateHeightsBuffer();
	MapTerrain()->updateTextureBuffer();
}

void Game::Map::CreateNoise(void) 
{
	mapgen::generateRandomMap();
	MapTerrain()->updateHeightsBuffer();

	stringstream ss;
	ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

	Logger::Info("Terrain has been generated!");
	Logger::Info(ss.str());
}

void Game::Map::UpdateGrid(void) 
{
	MapGrid()->update();
}

void Game::Map::Render(const bool tracing) 
{
	MapTerrain()->render(tracing);
	if (isGridEnabled && !tracing) 
	{
		MapGrid()->render();
	}
}

Game::Map::~Map(void) {}

#pragma endregion

#pragma region Selection rectangle class

Game::SelectionRectangle::SelRectPoints Game::SelectionRectangle::Coordinates;
float Game::SelectionRectangle::cameraLastX = 0.f;
float Game::SelectionRectangle::cameraLastY = 0.f;
bool Game::SelectionRectangle::isActive = false;
gui::Rectangle Game::SelectionRectangle::selRectangle;

void Game::SelectionRectangle::Disable(void)
{
	Game::SelectionRectangle::isActive = false;
}

void Game::SelectionRectangle::ResetExtremeValues(void)
{
	Coordinates.minX = -0.1f;
	Coordinates.maxX = -0.1f;
	Coordinates.minY = -0.1f;
	Coordinates.maxY = -0.1f;
}

void Game::SelectionRectangle::SetExtremeValues(void)
{
	Coordinates.minX = std::min(Coordinates.startX, Coordinates.lastX);
	Coordinates.maxX = std::max(Coordinates.startX, Coordinates.lastX);
	Coordinates.minY = std::min(Coordinates.startY, Coordinates.lastY);
	Coordinates.maxY = std::max(Coordinates.startY, Coordinates.lastY);
}

void Game::SelectionRectangle::CalculateSizeAndOrigin(float * width, float * height, int * origin)
{
	float _width = Coordinates.lastX - Coordinates.startX;
	float _height = Coordinates.lastY - Coordinates.startY;

	int _origin = 0;
	if (_width < 0 && _height < 0) _origin = TOPRIGHT_ORIGIN;
	else if (_width > 0 && _height > 0) _origin = BOTTOMLEFT_ORIGIN; 
	else if (_width > 0 && _height < 0) _origin = TOPLEFT_ORIGIN; 
	else _origin = BOTTOMRIGHT_ORIGIN; 

	(*width) = std::abs(_width);
	(*height) = std::abs(_height);
	(*origin) = _origin;
}

Game::SelectionRectangle::SelectionRectangle(void) {}

void Game::SelectionRectangle::Create(void) 
{
	cameraLastX = 0.f;
	cameraLastY = 0.f;
	selRectangle = gui::Rectangle();
	selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
}

bool Game::SelectionRectangle::IsInRectangle(array<float, 8> &coords) {
	if (IsActive() == false) return false;
	return(
		// are the 4 points in selection rectangle ?
		(coords[0] > Coordinates.minX && coords[0] < Coordinates.maxX &&
			coords[1] > Coordinates.minY && coords[1] < Coordinates.maxY) ||
			(coords[2] > Coordinates.minX && coords[2] < Coordinates.maxX &&
				coords[3] > Coordinates.minY && coords[3] < Coordinates.maxY) ||
				(coords[4] > Coordinates.minX && coords[4] < Coordinates.maxX &&
					coords[5] > Coordinates.minY && coords[5] < Coordinates.maxY) ||
					(coords[6] > Coordinates.minX && coords[6] < Coordinates.maxX &&
						coords[7] > Coordinates.minY && coords[7] < Coordinates.maxY) ||
		// does the sel. rectangle and hitbox rectangle intersect themselves?
						(coords[0] > Coordinates.minX && coords[0] < Coordinates.maxX &&
							coords[3] < Coordinates.minY && coords[1] > Coordinates.maxY) ||
							(coords[4] > Coordinates.minX && coords[4] < Coordinates.maxX &&
								coords[3] < Coordinates.minY && coords[1] > Coordinates.maxY) ||
								(coords[1] > Coordinates.minY && coords[1] < Coordinates.maxY &&
									coords[0] < Coordinates.minX && coords[4] > Coordinates.maxX) ||
									(coords[3] > Coordinates.minY && coords[3] < Coordinates.maxY &&
										coords[0] < Coordinates.minX && coords[4] > Coordinates.maxX)
		);
}

void Game::SelectionRectangle::Render(void) 
{
	if (Engine::Mouse::LeftClick) {
		if (SelectionRectangle::IsActive() == false) {
			Logger::Info("Selection rectangle enabled.");
			Coordinates.startX = Engine::Mouse::GetXLeftClick() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + cameraLastX;
			Coordinates.startY = Engine::Mouse::GetYLeftClick() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + cameraLastY;
		}
	}
	if (Engine::Mouse::LeftHold) {
		Coordinates.lastX = Engine::Mouse::GetXMapCoordinate();
		Coordinates.lastY = Engine::Mouse::GetYMapCoordinate();
		if (Engine::Mouse::GetYPosition() < Engine::myWindow::BottomBarHeight) {
			Coordinates.lastY = Engine::myWindow::BottomBarHeight * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + 1.0f + Engine::Camera::GetYPosition();
		}
		if (Engine::Mouse::GetYPosition() > Engine::myWindow::Height - Engine::myWindow::TopBarHeight) {
			Coordinates.lastY = Engine::myWindow::HeightZoomed - Engine::myWindow::TopBarHeight*Engine::myWindow::HeightZoomed / Engine::myWindow::Height - 1.0f + Engine::Camera::GetYPosition();
		}

		float w, h;
		int origin;
		CalculateSizeAndOrigin(&w, &h, &origin);

		if (w > 1 && h > 1) {
			selRectangle.render(vec4(255.f), 0, 0, Coordinates.startX, Coordinates.startY, w, h, origin);
			SetExtremeValues();
		}
		else {
			ResetExtremeValues();
		}
		SelectionRectangle::Enable();
	}
	else {
		if (SelectionRectangle::IsActive()) Logger::Info("Selection rectangle disabled.");
		cameraLastX = Engine::Camera::GetXPosition();
		cameraLastY = Engine::Camera::GetYPosition();
		ResetExtremeValues();
		SelectionRectangle::Disable();
	}
}

bool Game::SelectionRectangle::IsActive(void)
{
	return Game::SelectionRectangle::isActive;
}

void Game::SelectionRectangle::Enable(void)
{
	Game::SelectionRectangle::isActive = true;
}

#pragma endregion

#pragma region Minimap prerendered class

bool Game::Minimap::isCreated = false;
bool Game::Minimap::isActive = false;
bool Game::Minimap::isBlocked = false;
gui::Rectangle Game::Minimap::minimapRectangle;

Game::Minimap::Minimap(void) {
	minimapRectangle = gui::Rectangle();
}

void Game::Minimap::Create(void) 
{
	applyGameMatrices(&projectionMatrix, &viewMatrix);
	Map::Render(false);
	RenderObjects();
	MMRectangle()->update();
	isCreated = true;
}

void Game::Minimap::RenderMapAndObjects(void)
{
	if (IsCreated() == false) {
		Minimap::Create();
	}
	MMRectangle()->render();
}

void Game::Minimap::RenderRectangle(void)
{
	minimapRectangle.render(vec4(255.f));
}

void Game::Minimap::Update(void)
{
	Game::Minimap::isCreated = false;
}

bool Game::Minimap::IsCreated(void)
{
	return Game::Minimap::isCreated;
}

void Game::Minimap::Enable(void)
{
	Game::Minimap::isActive = true;

	float x = Engine::Camera::GetXPosition() / MEDIUM_MAP_WIDTH * Engine::myWindow::Width;
	float y = Engine::Camera::GetYPosition() / MEDIUM_MAP_HEIGHT * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) + Engine::myWindow::BottomBarHeight;
	float w = Engine::myWindow::WidthZoomed * Engine::myWindow::Width / MEDIUM_MAP_WIDTH;
	float h = Engine::myWindow::HeightZoomed * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) / MEDIUM_MAP_HEIGHT;
	x = std::max(x, 1.f);
	y = std::max(y, Engine::myWindow::BottomBarHeight + 1.f);
	y = std::min(y, Engine::myWindow::Height - Engine::myWindow::TopBarHeight - h);
	minimapRectangle.create("border", x, y, w, h, "bottom-left", 0);
}

void Game::Minimap::Disable(void)
{
	Game::Minimap::isActive = false;
}

bool Game::Minimap::IsActive(void)
{
	return Game::Minimap::isActive;
}

bool Game::Minimap::IsBlocked(void)
{
	return Game::Minimap::isBlocked;
}

void Game::Minimap::Block(void)
{
	Game::Minimap::isBlocked = true;
}

void Game::Minimap::Unblock(void)
{
	Game::Minimap::isBlocked = false;
}

Game::Minimap::~Minimap(void) {}

#pragma endregion

#pragma region Race class

Game::Race::Race() { };
Game::Race::~Race() { };

#pragma endregion


#pragma region Static variables

glm::mat4 Game::projectionMatrix;
glm::mat4 Game::viewMatrix;
bool Game::isCreated = false;
float Game::cameraToX;
float Game::cameraToY;
int Game::numberOfPlayers = 1;
vector<string> Game::racesName;
map<string, Game::Race> Game::races;
vector<vec3> Game::listOfColors;
GObject* Game::selectedObject;

#pragma endregion

Game::Game() {}

void Game::ResetGame()
{
	GObject::ResetGameObjects();
	//game::GameMenu::Disable();
	Map::DisableGrid();
	Minimap::Unblock();
	Minimap::Disable();
}

Game::~Game() {}

;
bool Game::IsGameObjectSelected(const unsigned int id)
{
	if (IsGameObjectNotNull(id) == false) return false;
	return GObject::GetObjectByID(id)->IsSelected();
}

bool Game::IsGameObjectNotNull(int id) 
{
	if (id < 0 || id > MAX_NUMBER_OF_OBJECTS)
		return false;
	else
		return (GObject::GetObjectByID(id) != nullptr);
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

void Game::RenderObjectsPicking() {
 	if (Picking::leftClickID_UI != 0) {
		Picking::leftClickID = 0;
		Picking::rightClickID = 0;
		return;
	}

	if ((Engine::Mouse::RightClick || Engine::Mouse::LeftClick) && !SelectionRectangle::IsActive()) {

		for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
			if (GObject::GetObjectByID(i) != nullptr) {
				GObject::GetObjectByID(i)->Render(true);
			}
		}

		if (Engine::Mouse::LeftClick)
		{
			Picking::leftClickID = Picking::GetIdFromClick(PICKING_LEFT);
		}
		if (Engine::Mouse::RightClick) {
			Picking::rightClickID = Picking::GetIdFromClick(PICKING_RIGHT);
		}

		if (Minimap::IsActive()) {
			Minimap::Unblock();
			if (Picking::leftClickID > 0) {
				Minimap::Block();
			}
		}
	}
}

void Game::RenderObjects() 
{
	for (unsigned int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) 
	{
		if (GObject::GetObjectByID(i) != nullptr) 
		{
			GObject::GetObjectByID(i)->Render(false, Picking::leftClickID);
		}
	}

	if (!Minimap::IsActive() && Picking::leftClickID_UI == 0) SelectionRectangle::Render(); //&& !editor::movingObject
}

void Game::GoToPointFromMinimap() {
	if (Engine::Mouse::LeftClick && Engine::Mouse::IsCursorInGameScreen()) {
		cameraToX = Engine::Mouse::GetXLeftClick() / Engine::myWindow::Width*(float)MEDIUM_MAP_WIDTH - Engine::myWindow::WidthZoomed / 2.f;
		cameraToY = Engine::Camera::GetYMinimapCoordinate(Engine::Mouse::GetYLeftClick()) / Engine::myWindow::Height*(float)MEDIUM_MAP_HEIGHT - Engine::myWindow::HeightZoomed / 2.f;
		// if you are clicking on a townhall you have to double click 
		// to move the camera there and quit minimap
		if (Picking::leftClickID > 0 && Picking::HasDoubleClicked()) {
			cameraToX = GObject::GetObjectByID(Picking::leftClickID)->AsBuilding()->get_xPos() - Engine::myWindow::WidthZoomed / 2.f;
			cameraToY = GObject::GetObjectByID(Picking::leftClickID)->AsBuilding()->get_yPos() - Engine::myWindow::HeightZoomed / 2.f;
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
		b->SetPickingID(PickingObject::ObtainPickingID());
		b->GetSettlement().SetSettlementName("Outpost_" + i);
		b->create();
		GameObjects[b->GetPickingID()] = b;
	}*/
}

#pragma region Map Generation Methods

void Game::GenerateSettlements(vector<vec2> &locs) {
	string RandomMapSettlementPath = "scenarios/RandomMapSettlements/";

	vector<int> TemporaryMapTextures;

	int num_townhalls = (int)locs.size();

	// read texture
	{
		fstream fin;
		fin.open(RandomMapSettlementPath + "texture");
		string line, number;
		getline(fin, line);
		stringstream s(line);
		int i = 0;
		while (getline(s, number, ',')) {
			TemporaryMapTextures.push_back(stoi(number));
			i++;
		}
	}

	for (int i = 0; i < num_townhalls; i++) {
		string r = playersList[i].GetPlayerRace().substr(5);
		vec2 origin = locs[i];
		playersList[i].SetStartPoint(origin);
		string SettlementRace = "Settlement_" + r + "_1";
		fstream fin;
		fin.open(RandomMapSettlementPath + "objects.tsv");
		string line, value;
		int row = 0;
		while (getline(fin, line)) {
			if (row > 0) {
				string objectsInfo[9];
				stringstream s(line);
				int v = 0;
				while (getline(s, value, '\t')) { objectsInfo[v] = value; v++; }
				string settl_name = objectsInfo[3];
				if (settl_name == SettlementRace) {
					string type = objectsInfo[0];
					string className = objectsInfo[1];
					string settl_name = objectsInfo[3];
					float xOffset = stof(objectsInfo[7]);
					float yOffset = stof(objectsInfo[8]);

					float xEditorPos = stof(objectsInfo[5]);
					float yEditorPos = stof(objectsInfo[6]);

					if (type == "building") {
						//Building* b = new Building();
						//b->SetClassName(className);
						//b->SetType(type);
						//b->SetPlayer(i);
						//b->SetPosition(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						//b->SetPickingID(PickingObject::ObtainPickingID());
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
					}
					if (type == "decoration") {
						/*Decoration* d = new Decoration();
						d->SetClassName(className);
						d->SetPlayer(0);
						d->SetPosition(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						d->SetPickingID(PickingObject::ObtainPickingID());
						d->create();
						GameObjects[d->GetPickingID()] = d;*/
					}
				}
			}
			row++;
		}
	}

	// update texture buffer
	MapTerrain()->updateTextureBuffer();

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
			newBuilding->SetPosition(vec3(x, y, 10));
			newBuilding->SetPlayer(player);			
			bObjectCreated = newBuilding->Create(className);
			if (bObjectCreated == false)
			{
				delete newBuilding;
				newBuilding = nullptr;
			}
		}
		if (type == "cpp_decorationclass") {
			Decoration* newDecoration = new Decoration();
			newDecoration->SetPosition(vec3(x, y, 10));
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

