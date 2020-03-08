#include "game.h"
#include <surface>
#include <global>
#include "strategy.h"
#include <picking>
#include <math>
#include <engine/mouse.h>
#include <engine/window.h>
#include <engine/camera.h>
#include <interface>
#include "editor.h"

#pragma region Namespaces

using namespace glb;
using namespace engine;

#pragma endregion


#pragma region Surface

bool Game::Surface::Wireframe = false;
bool Game::Surface::isGridEnabled;

Game::Surface::Surface() {}

void Game::Surface::Reset() {
	isGridEnabled = false;
	mapgen::reset_map();
	obj::MapGrid()->reset();
	obj::MapTerrain()->updateHeightsBuffer();
	obj::MapTerrain()->updateTextureBuffer();
}

void Game::Surface::CreateNoise() {
	mapgen::generateRandomMap();
	obj::MapTerrain()->updateHeightsBuffer();

	stringstream ss;
	ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

	Logger::Info("Terrain has been generated!");
	Logger::Info(ss.str());
}

void Game::Surface::UpdateGrid() {
	obj::MapGrid()->update();
}

void Game::Surface::Render(bool tracing) {

	obj::MapTerrain()->render(tracing);

	if (isGridEnabled && !tracing) {
		obj::MapGrid()->render();
	}
}

Game::Surface::~Surface() {}

#pragma endregion

#pragma region Selection rectangle

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
	if (Mouse::LeftHold) {

		if (SelectionRectangle::IsActive() == false) {
			Logger::Info("Selection rectangle enabled.");
			Coordinates.startX = Mouse::GetXLeftClick() * myWindow::WidthZoomed / myWindow::Width + cameraLastX;
			Coordinates.startY = Mouse::GetYLeftClick() * myWindow::HeightZoomed / myWindow::Height + cameraLastY;
		}
		Coordinates.lastX = Mouse::GetXPosition() * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
		Coordinates.lastY = Mouse::GetYPosition() * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
		if (Mouse::GetYPosition() < myWindow::BottomBarHeight) {
			Coordinates.lastY = myWindow::BottomBarHeight*myWindow::HeightZoomed / myWindow::Height + 1.0f + Camera::GetYPosition();
		}
		if (Mouse::GetYPosition() > myWindow::Height - myWindow::TopBarHeight) {
			Coordinates.lastY = myWindow::HeightZoomed - myWindow::TopBarHeight*myWindow::HeightZoomed / myWindow::Height - 1.0f + Camera::GetYPosition();
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
		cameraLastX = Camera::GetXPosition();
		cameraLastY = Camera::GetYPosition();
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

#pragma region Minimap prerendered

bool Game::Minimap::isCreated = false;
bool Game::Minimap::isActive = false;
bool Game::Minimap::isBlocked = false;

Game::Minimap::Minimap() {}

void Game::Minimap::Create() {
	obj::MMRectangle()->update();
	isCreated = true;
}

void Game::Minimap::Render() {
	obj::MMRectangle()->render();
}

Game::Minimap::~Minimap() {}

#pragma endregion

#pragma region Static variables

glm::mat4 Game::projectionMatrix;
glm::mat4 Game::viewMatrix;
bool Game::isCreated = false;
float Game::cameraToX;
float Game::cameraToY;
int Game::numberOfPlayers = 1;
GObject* Game::GameObjects[MAX_NUMBER_OF_OBJECTS] = { nullptr };

#pragma endregion

Game::Game() {}
Game::~Game() {}

void Game::RemoveGameObject(int i) {
	if (i >= 1 && i < MAX_NUMBER_OF_OBJECTS) {
		if (GameObjects[i] != nullptr) {
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
	if (leftClickID_UI != 0) {
		leftClickID = 0;
		rightClickID = 0;
		return;
	}

	if ((Mouse::RightClick || Mouse::LeftClick) && !SelectionRectangle::IsActive()) {

		for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
			if (GameObjects[i] != nullptr) {
				GameObjects[i]->render(true);
			}
		}

		if (Mouse::LeftClick) leftClickID = get_id("left");
		if (Mouse::RightClick) rightClickID = get_id("right");

		if (Minimap::IsActive()) {
			Minimap::Unblock();
			if (leftClickID > 0) {
				Minimap::Block();
			}
		}
	}
}

void Game::RenderObjects() {
	//int selectedBuildings = 0;

	for (int i = 1; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GameObjects[i] != nullptr) {
			GameObjects[i]->render(false, leftClickID);
		}
	}
		/*cout << gameMinimapStatus << " " << editor::IsWindowOpened << " " << editor::menuIsOpened << " " << editor::TerrainBrushIsActive << " " << leftClickID_UI << editor::movingObject << endl;*/
	if (!Minimap::IsActive() && !editor::IsWindowOpened && !editor::menuIsOpened && !editor::TerrainBrushIsActive && leftClickID_UI == 0 && !editor::movingObject) SelectionRectangle::Render();
}

void Game::GoToPointFromMinimap() {
	if (Mouse::LeftClick && cursorInGameScreen()) {
		cameraToX = Mouse::GetXLeftClick() / myWindow::Width*(float)MEDIUM_MAP_WIDTH - myWindow::WidthZoomed / 2.f;
		cameraToY = getYMinimapCoord(Mouse::GetYLeftClick()) / myWindow::Height*(float)MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed / 2.f;
		// if you are clicking on a townhall you have to double click 
		// to move the camera there and quit minimap
		if (leftClickID > 0 && hasDoubleClicked()) {
			cameraToX = GameObjects[leftClickID]->AsBuilding()->get_xPos() - myWindow::WidthZoomed / 2.f;
			cameraToY = GameObjects[leftClickID]->AsBuilding()->get_yPos() - myWindow::HeightZoomed / 2.f;
			Game::Minimap::Unblock();
		}
		//------------------------------------------------
		if (Game::Minimap::IsBlocked() == false) {
			Camera::GoToPoint(cameraToX, cameraToY);
			Game::Minimap::Disable();
			Mouse::LeftClick = false;
			Mouse::LeftHold = false;
		}
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
						b->set_id(getPickingID());
						b->set_settlement_name("Settlement_player_" + i);
						b->create();
						GameObjects[getPickingID()] = b;

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
						increasePickingID();
					}
					if (type == "decoration") {
						Decoration* d = new Decoration();
						d->set_class(className);
						d->set_player(0);
						d->set_position(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						d->set_id(getPickingID());
						d->create();
						GameObjects[getPickingID()] = d;
						increasePickingID();
					}
				}
			}
			row++;
		}
	}

	// update texture buffer
	obj::MapTerrain()->updateTextureBuffer();

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

void Game::GenerateOutposts(vector<vec2> &locs) {

	string className = "routpost";

	for (int i = 0; i < locs.size(); i++) {
		Building* b = new Building();
		b->set_class(className);
		b->set_type("building");
		b->set_player(0);
		b->set_position(vec3(locs[i].x, locs[i].y, 0.f));
		b->set_id(getPickingID());
		b->set_settlement_name("Outpost_" + i);
		b->create();
		GameObjects[getPickingID()] = b;
		increasePickingID();
	}
}

#pragma endregion
