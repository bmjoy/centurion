#include "game.h"
#include <surface>
#include <global>
#include "strategy.h"
#include <picking.h>
#include <maths.h>
#include <engine.h>
#include <interface>
#include "editor.h"

#include <primitives.h>
#include <terrain.h>
#include <grid.h>
#include <empty_rectangle.h>
#include <filled_rectangle.h>
#include <minimap_rectangle.h>

#pragma region Namespaces

using namespace glb;


#pragma endregion


#pragma region Surface class

bool Game::Surface::Wireframe = false;
bool Game::Surface::isGridEnabled;

Game::Surface::Surface() {}

void Game::Surface::Reset() {
	isGridEnabled = false;
	mapgen::reset_map();
	MapGrid()->reset();
	MapTerrain()->updateHeightsBuffer();
	MapTerrain()->updateTextureBuffer();
}

void Game::Surface::CreateNoise() {
	mapgen::generateRandomMap();
	MapTerrain()->updateHeightsBuffer();

	stringstream ss;
	ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

	Logger::Info("Terrain has been generated!");
	Logger::Info(ss.str());
}

void Game::Surface::UpdateGrid() {
	MapGrid()->update();
}

void Game::Surface::Render(bool tracing) {

	MapTerrain()->render(tracing);

	if (isGridEnabled && !tracing) {
		MapGrid()->render();
	}
}

Game::Surface::~Surface() {}

#pragma endregion

#pragma region Selection rectangle class

Game::SelectionRectangle::SelRectPoints Game::SelectionRectangle::Coordinates;
float Game::SelectionRectangle::cameraLastX = 0.f;
float Game::SelectionRectangle::cameraLastY = 0.f;
bool Game::SelectionRectangle::isActive = false;
gui::Rectangle Game::SelectionRectangle::selRectangle;

Game::SelectionRectangle::SelectionRectangle() {}

void Game::SelectionRectangle::Create() {
	cameraLastX = 0.f;
	cameraLastY = 0.f;
	selRectangle = gui::Rectangle();
	selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
}

bool Game::SelectionRectangle::IsInRectangle(array<float, 8> &coords) {
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

void Game::SelectionRectangle::Render() {
	if (Engine::Mouse::LeftHold) {

		if (SelectionRectangle::IsActive() == false) {
			Logger::Info("Selection rectangle enabled.");
			Coordinates.startX = Engine::Mouse::GetXLeftClick() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + cameraLastX;
			Coordinates.startY = Engine::Mouse::GetYLeftClick() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + cameraLastY;
		}
		Coordinates.lastX = Engine::Mouse::GetXPosition() * Engine::myWindow::WidthZoomed / Engine::myWindow::Width + Engine::Camera::GetXPosition();
		Coordinates.lastY = Engine::Mouse::GetYPosition() * Engine::myWindow::HeightZoomed / Engine::myWindow::Height + Engine::Camera::GetYPosition();
		if (Engine::Mouse::GetYPosition() < Engine::myWindow::BottomBarHeight) {
			Coordinates.lastY = Engine::myWindow::BottomBarHeight*Engine::myWindow::HeightZoomed / Engine::myWindow::Height + 1.0f + Engine::Camera::GetYPosition();
		}
		if (Engine::Mouse::GetYPosition() > Engine::myWindow::Height - Engine::myWindow::TopBarHeight) {
			Coordinates.lastY = Engine::myWindow::HeightZoomed - Engine::myWindow::TopBarHeight*Engine::myWindow::HeightZoomed / Engine::myWindow::Height - 1.0f + Engine::Camera::GetYPosition();
		}

		float w = (Coordinates.lastX - Coordinates.startX);
		float h = (Coordinates.lastY - Coordinates.startY);

		int origin = 0;
		if (w > 0 && h > 0) origin = 0; // bottom-left
		if (w > 0 && h < 0) origin = 1; // top-left
		if (w < 0 && h > 0) origin = 4; // bottom-right
		if (w < 0 && h < 0) origin = 3; // top-right

		if (abs(w) > 1 && abs(h) > 1) {
			selRectangle.render(vec4(255.f), 0, Coordinates.startX, Coordinates.startY, abs(w), abs(h), origin);
			Coordinates.minX = std::min(Coordinates.startX, Coordinates.lastX);
			Coordinates.maxX = std::max(Coordinates.startX, Coordinates.lastX);
			Coordinates.minY = std::min(Coordinates.startY, Coordinates.lastY);
			Coordinates.maxY = std::max(Coordinates.startY, Coordinates.lastY);
		}
		else {
			Coordinates.minX = -0.1f;
			Coordinates.maxX = -0.1f;
			Coordinates.minY = -0.1f;
			Coordinates.maxY = -0.1f;
		}
		SelectionRectangle::Enable();
	}
	else {
		if (SelectionRectangle::IsActive()) Logger::Info("Selection rectangle disabled.");
		cameraLastX = Engine::Camera::GetXPosition();
		cameraLastY = Engine::Camera::GetYPosition();
		Coordinates.startX = -0.1f;
		Coordinates.startY = -0.1f;
		Coordinates.lastX = -0.1f;
		Coordinates.lastY = -0.1f;
		Coordinates.minX = -0.1f;
		Coordinates.maxX = -0.1f;
		Coordinates.minY = -0.1f;
		Coordinates.maxY = -0.1f;
		SelectionRectangle::Disable();
	}
}

#pragma endregion

#pragma region Minimap prerendered class

bool Game::Minimap::isCreated = false;
bool Game::Minimap::isActive = false;
bool Game::Minimap::isBlocked = false;

Game::Minimap::Minimap() {}

void Game::Minimap::Create() {
	MMRectangle()->update();
	isCreated = true;
}

void Game::Minimap::Render() {
	MMRectangle()->render();
}

Game::Minimap::~Minimap() {}

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
GObject* Game::GameObjects[MAX_NUMBER_OF_OBJECTS] = { nullptr };
vector<string> Game::racesName;
map<string, Game::Race> Game::races;
vector<vec3> Game::listOfColors;

#pragma endregion

Game::Game() {}
Game::~Game() {}

void Game::RemoveGameObject(int i) {
	if (i >= 1 && i < MAX_NUMBER_OF_OBJECTS) {
		if (GameObjects[i] != nullptr) {
			PickingObject::addUnsedPickingID(GameObjects[i]->get_id());
			delete GameObjects[i];
		}
		GameObjects[i] = nullptr;
	}
}

void Game::ResetGameObjects() {
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr) {
			delete GameObjects[i];
		}
		GameObjects[i] = nullptr;
	}
}

int Game::GetNumberOfGameObjects() {
	int n = 0;
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr) {
			n++;
		}
	}
	return n;
}

int Game::GetNumberOfBuildings() {
	int n = 0;
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsBuilding()) {
			n++;
		}
	}
	return n;
}

int Game::GetNumberOfUnits() {
	int n = 0;
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsUnit()) {
			n++;
		}
	}
	return n;
}

int Game::GetNumberOfDecorations() {
	int n = 0;
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsDecoration()) {
			n++;
		}
	}
	return n;
}

bool Game::IsGameObjectNotNull(int i) {
	return (GameObjects[i] != nullptr);
}

vector<Building*> Game::GetListOfIndipendentBuildings() {
	vector<Building*> indipBuildings = vector<Building*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsBuilding()) {
			if (GameObjects[i]->AsBuilding()->is_independent()) {
				indipBuildings.push_back(GameObjects[i]->AsBuilding());
			}
		}
	}
	return indipBuildings;
}

vector<Building*> Game::GetListOfBuildings() {
	vector<Building*> indipBuildings = vector<Building*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsBuilding()) {
			indipBuildings.push_back(GameObjects[i]->AsBuilding());
		}
	}
	return indipBuildings;
}
vector<Unit*> Game::GetListOfUnits() {
	vector<Unit*> output = vector<Unit*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsUnit()) {
			output.push_back(GameObjects[i]->AsUnit());
		}
	}
	return output;
}
vector<Decoration*> Game::GetListOfDecorations() {
	vector<Decoration*> output = vector<Decoration*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr && GameObjects[i]->IsBuilding()) {
			output.push_back(GameObjects[i]->AsDecoration());
		}
	}
	return output;
}

void Game::RenderObjectsPicking() {
	if (Picking::leftClickID_UI != 0) {
		Picking::leftClickID = 0;
		Picking::rightClickID = 0;
		return;
	}

	if ((Engine::Mouse::RightClick || Engine::Mouse::LeftClick) && !SelectionRectangle::IsActive()) {

		for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
			if (GameObjects[i] != nullptr) {
				GameObjects[i]->render(true);
			}
		}

		if (Engine::Mouse::LeftClick) Picking::leftClickID = Picking::GetIdFromClick(PICKING_LEFT);
		if (Engine::Mouse::RightClick) Picking::rightClickID = Picking::GetIdFromClick(PICKING_RIGHT);

		if (Minimap::IsActive()) {
			Minimap::Unblock();
			if (Picking::leftClickID > 0) {
				Minimap::Block();
			}
		}
	}
}

void Game::RenderObjects() {
	//int selectedBuildings = 0;

	for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr) {
			GameObjects[i]->render(false, Picking::leftClickID);
		}
	}
		/*cout << gameMinimapStatus << " " << editor::IsWindowOpened << " " << editor::menuIsOpened << " " << editor::TerrainBrushIsActive << " " << leftClickID_UI << editor::movingObject << endl;*/
	if (!Minimap::IsActive() && !editor::IsWindowOpened && !editor::menuIsOpened && !editor::TerrainBrushIsActive && Picking::leftClickID_UI == 0 && !editor::movingObject) SelectionRectangle::Render();
}

void Game::GoToPointFromMinimap() {
	if (Engine::Mouse::LeftClick && cursorInGameScreen()) {
		cameraToX = Engine::Mouse::GetXLeftClick() / Engine::myWindow::Width*(float)MEDIUM_MAP_WIDTH - Engine::myWindow::WidthZoomed / 2.f;
		cameraToY = getYMinimapCoord(Engine::Mouse::GetYLeftClick()) / Engine::myWindow::Height*(float)MEDIUM_MAP_HEIGHT - Engine::myWindow::HeightZoomed / 2.f;
		// if you are clicking on a townhall you have to double click 
		// to move the camera there and quit minimap
		if (Picking::leftClickID > 0 && Picking::hasDoubleClicked()) {
			cameraToX = GameObjects[Picking::leftClickID]->AsBuilding()->get_xPos() - Engine::myWindow::WidthZoomed / 2.f;
			cameraToY = GameObjects[Picking::leftClickID]->AsBuilding()->get_yPos() - Engine::myWindow::HeightZoomed / 2.f;
			Game::Minimap::Unblock();
		}
		//------------------------------------------------
		if (Game::Minimap::IsBlocked() == false) {
			Engine::Camera::GoToPoint(cameraToX, cameraToY);
			Game::Minimap::Disable();
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

void Game::GenerateOutposts(vector<vec2> &locs) {

	string className = "routpost";

	for (int i = 0; i < locs.size(); i++) {
		Building* b = new Building();
		b->set_class(className);
		b->set_type("building");
		b->set_player(0);
		b->set_position(vec3(locs[i].x, locs[i].y, 0.f));
		b->set_id(PickingObject::GetPickingId());
		b->set_settlement_name("Outpost_" + i);
		b->create();
		GameObjects[b->get_id()] = b;
	}
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
		string r = playersList[i].getPlayerRace().substr(5);
		vec2 origin = locs[i];
		playersList[i].setStartPoint(origin);
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
						Building* b = new Building();
						b->set_class(className);
						b->set_type(type);
						b->set_player(i);
						b->set_position(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						b->set_id(PickingObject::GetPickingId());
						b->set_settlement_name("Settlement_player_" + i);
						b->create();
						GameObjects[b->get_id()] = b;

						if (b->is_independent()) {
							// update terrain around the townhall
							// N.B: mapgen::grid_size * 2 because the map has "borders"

							int EditorStartPointX = (int)xEditorPos - TOWNHALL_RADIUS + mapgen::grid_size * 2;
							int EditorStartPointY = (int)yEditorPos - TOWNHALL_RADIUS + mapgen::grid_size * 2;
							int NewMapStartPointX = (int)origin.x - TOWNHALL_RADIUS + mapgen::grid_size * 2;
							int NewMapStartPointY = (int)origin.y - TOWNHALL_RADIUS + mapgen::grid_size * 2;

							for (int iHoriz = 0; iHoriz <= TOWNHALL_RADIUS * 2; iHoriz += mapgen::grid_size) {
								for (int iVert = 0; iVert <= TOWNHALL_RADIUS * 2; iVert += mapgen::grid_size) {

									int EditorPointX = EditorStartPointX + iHoriz;
									int EditorPointY = EditorStartPointY + iVert;
									int NewMapPointX = NewMapStartPointX + iHoriz;
									int NewMapPointY = NewMapStartPointY + iVert;

									if (math::euclidean_distance((float)NewMapPointX, (float)NewMapPointY, origin.x + mapgen::grid_size * 2.f, origin.y + mapgen::grid_size * 2.f) > TOWNHALL_RADIUS) continue;

									int EditorPointLoc = mapgen::getVertexPos(EditorPointX, EditorPointY);
									int NewMapPointLoc = mapgen::getVertexPos(NewMapPointX, NewMapPointY);
									mapgen::MapTextures()[NewMapPointLoc] = float(TemporaryMapTextures[EditorPointLoc]);
								}
							}
						}
					}
					if (type == "decoration") {
						Decoration* d = new Decoration();
						d->set_class(className);
						d->set_player(0);
						d->set_position(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						d->set_id(PickingObject::GetPickingId());
						d->create();
						GameObjects[d->get_id()] = d;
					}
				}
			}
			row++;
		}
	}

	// update texture buffer
	MapTerrain()->updateTextureBuffer();

	// update buildings info
	UpdateSettlementBuildings();
}

void Game::UpdateSettlementBuildings() {
	vector<Building*> listOfBuildings = Game::GetListOfBuildings();
	vector<Building*> listOfIndipBuildings = Game::GetListOfIndipendentBuildings();

	for (int i = 0; i < listOfBuildings.size(); i++) {
		Building* bld = listOfBuildings[i];
		if (!bld->is_independent()) {
			for (int j = 0; j < listOfIndipBuildings.size(); j++) {
				Building* settl = listOfIndipBuildings[j];
				if (settl->get_settlement_name() == bld->get_settlement_name()) {
					bld->set_settlement_building(settl);
					break;
				}
			}
		}
	}
}

#pragma endregion
