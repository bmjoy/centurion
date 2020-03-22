#include "game.h"
#include <mapgen/mapgen.h>

#include "strategy.h"
#include <picking.h>
#include <maths.h>
#include <engine.h>
#include <interface>
#include "editor.h"

#include <terrain.h>
#include <grid.h>
#include <gl_rectangle.h>
#include <minimap_rectangle.h>

#include "mapObjects-xml.hxx"

#include <logger.h>
#include <settings.h>
#include <classes/object-data.h>

#pragma region Map class

bool Game::Map::Wireframe = false;
bool Game::Map::isGridEnabled;

Game::Map::Map() {}

void Game::Map::LoadScenario(string scenarioName)
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

void Game::Map::SaveScenario(string scenarioName)
{
	try
	{
		string scenarioPath = Folders::SCENARIOS + scenarioName;

		if (FileManager::CheckIfFolderExists(scenarioPath) == false) {
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

void Game::Map::SaveMapObjectsToXml(string xmlPath)
{
	try
	{
		c_buildings _buildings = c_buildings();

		/* settlements */

		vector<Building*> indip_bs = GetListOfIndipendentBuildings();

		for (int i = 0; i < indip_bs.size(); i++) {

			Building* gobj = indip_bs[i];

			c_settlement _settl = c_settlement((c_settlement::name_type)gobj->GetSingularName(),
				(c_settlement::player_type)1,
				(c_settlement::x_type)gobj->GetPosition().x,
				(c_settlement::y_type)gobj->GetPosition().y
			);

			c_building1 b = c_building1((c_building1::class_type) gobj->GetClassName(),
				(c_building1::id_type)gobj->GetPickingID(),
				(c_building1::xOffset_type)0,
				(c_building1::yOffset_type)0
			);
			b.healthperc(100);
			b.name(gobj->GetSingularName());
			_settl.c_building().push_back(b);

			/*
			vector<int> dip_bs = gobj->buildingsInSettlementIds();
			for (int i = 0; i < dip_bs.size(); i++) {
				int id = dip_bs[i];
				Building* gobj2 = GObject::GetObjectByID(id)->AsBuilding();

				c_building1 b2 = c_building1((c_building1::class_type) gobj2->GetClassName(),
					(c_building1::id_type)gobj2->GetPickingID(),
					c_building1::xOffset_type(gobj2->GetPosition().x - gobj->GetPosition().x),
					c_building1::yOffset_type(gobj2->GetPosition().y - gobj->GetPosition().y)
				);
				b2.healthperc(100);
				b2.name(gobj2->GetName());
				_settl.c_building().push_back(b2);
			}
			*/
			_buildings.c_settlement().push_back(_settl);
		}

		//vector<Building*> indip_bs = GetListOfIndipendentBuildings();

		//for (int i = 0; i < indip_bs.size(); i++) {

		//	c_settlement _settl = c_settlement((c_settlement::name_type)gobj->GetClassName(),

		//		);

		//	if (indip_bs[i]->IsBuilding()) {

		//		Building* gobj = indip_bs[i];

		//		c_building b = c_building((c_building::class_type) gobj->GetClassName(),
		//			(c_building::id_type)gobj->GetPickingID(),
		//			(c_building::player_type)1,
		//			(c_building::x_type)gobj->GetPosition().x,
		//			(c_building::y_type)gobj->GetPosition().y,
		//			(c_building::gold_type)100,
		//			(c_building::food_type)100
		//		);

		//		b.healthperc(100);
		//		b.name(gobj->GetName());
		//		//b.icon = "";

		//		_buildings.c_building().push_back(b);
		//	}
		//}

		c_decorations _decorations = c_decorations();

		vector<Decoration*> _decors = GObject::GetListOfDecorations();
		for (int i = 0; i < _decors.size(); i++) {
			c_decoration _dec = c_decoration(
				c_decoration::class_type(_decors[i]->GetClassName()),
				c_decoration::id_type(_decors[i]->GetPickingID()),
				c_decoration::x_type(_decors[i]->GetPosition().x),
				c_decoration::y_type(_decors[i]->GetPosition().y)
			);

			_decorations.c_decoration().push_back(_dec);
		}

		c_units _units = c_units();

		c_mapObjects mapObjs = c_mapObjects(_buildings, _decorations, _units);

		xml_schema::namespace_infomap map;
		map[""].schema = "";
		ofstream ofs(xmlPath.c_str());
		c_mapObjects_(ofs, mapObjs, map);
	}
	catch (const xml_schema::exception & e) {
		std::cout << e << std::endl;
		Engine::GameClose();
	}
	catch (...)
	{
		Engine::GameClose();
	}
}

void Game::Map::SaveHeights(string path)
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

void Game::Map::SaveTexture(string path)
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

void Game::Map::LoadMapObjectsFromXml(string xmlPath)
{
	try
	{
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "mapObjects.xsd");
		auto_ptr<c_mapObjects> mapObjs = c_mapObjects_(xmlPath, 0, props);

		// buildings 

		// independent buildings and their ...

		/*c_buildings::c_settlement_iterator _settl;
		for (_settl = mapObjs->c_buildings().c_settlement().begin(); _settl != mapObjs->c_buildings().c_settlement().end(); _settl++) {

			c_settlement::c_building_iterator _bld;
			for (_bld = _settl->c_building().begin(); _bld != _settl->c_building().end(); _bld++) {

				Building* b = new Building();
				b->SetClassName((string)_bld->class_());
				b->SetPlayer((const unsigned short)_settl->player());
				b->SetPosition(vec3(_settl->x() - _bld->xOffset(), _settl->y() - _bld->yOffset(), 0.f));
				b->SetPickingID(PickingObject::GetPickingId());
				b->GetSettlement().SetSettlementName(_settl->name());
				b->SetType("building");
				b->create(_bld->name().get());
				Game::AddGameObject(b->GetPickingID(), b);
			}
		}*/

		// stand alone buildings!!


		// decorations

		/*c_decorations::c_decoration_iterator _dec;
		for (_dec = mapObjs->c_decorations().c_decoration().begin(); _dec != mapObjs->c_decorations().c_decoration().end(); _dec++) {
			
			Decoration* d = new Decoration();
			d->SetClassName(_dec->class_());
			d->SetPlayer(0);
			d->SetPosition(vec3(_dec->x(), _dec->y(), 0.f));
			d->SetPickingID(PickingObject::GetPickingId());
			d->create();
			d->SetType("decoration");
			Game::AddGameObject(d->GetPickingID(), d);
		}*/

		// units
	}
	catch (const xml_schema::exception & e) {
		string emsg = string(e.what());
		Logger::LogMessage msg = Logger::LogMessage(emsg, "", "Game::Map", "LoadMapObjectsFromXml");
		Logger::Error(msg);
		Engine::GameClose();
	}
	catch (...)
	{
		Engine::GameClose();
	}
}



void Game::Map::LoadHeights(string path)
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

void Game::Map::LoadTexture(string path)
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

void Game::Map::Reset() {
	isGridEnabled = false;
	mapgen::reset_map();
	MapGrid()->reset();
	MapTerrain()->updateHeightsBuffer();
	MapTerrain()->updateTextureBuffer();
}

void Game::Map::CreateNoise() {
	mapgen::generateRandomMap();
	MapTerrain()->updateHeightsBuffer();

	stringstream ss;
	ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

	Logger::Info("Terrain has been generated!");
	Logger::Info(ss.str());
}

void Game::Map::UpdateGrid() {
	MapGrid()->update();
}

void Game::Map::Render(bool tracing) {

	MapTerrain()->render(tracing);

	if (isGridEnabled && !tracing) {
		MapGrid()->render();
	}
}

Game::Map::~Map() {}

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
			selRectangle.render(vec4(255.f), 0, 0, Coordinates.startX, Coordinates.startY, abs(w), abs(h), origin);
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
vector<string> Game::racesName;
map<string, Game::Race> Game::races;
vector<vec3> Game::listOfColors;
GObject* Game::selectedObject;

#pragma endregion

Game::Game() {}
Game::~Game() {}

;
bool Game::IsGameObjectSelected(const unsigned int id)
{
	if (IsGameObjectNotNull(id) == false) return false;
	return GObject::GetObjectByID(id)->IsSelected();
}

bool Game::IsGameObjectNotNull(int id) {
	if (id < 0 || id > MAX_NUMBER_OF_OBJECTS)
		return false;
	else
		return (GObject::GetObjectByID(id) != nullptr);
}


vector<Building*> Game::GetListOfIndipendentBuildings() {
	vector<Building*> indipBuildings = vector<Building*>();
	for (int i = 0; i < MAX_NUMBER_OF_OBJECTS; i++) {
		if (GObject::GetObjectByID(i) != nullptr && GObject::GetObjectByID(i)->IsBuilding()) {
			if (GObject::GetObjectByID(i)->AsBuilding()->GetSettlement()->IsIndipendent()) {
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
				GObject::GetObjectByID(i)->render(true);
			}
		}

		if (Engine::Mouse::LeftClick)
		{
			Picking::leftClickID = Picking::GetIdFromClick(PICKING_LEFT);
		}
		if (Engine::Mouse::RightClick) Picking::rightClickID = Picking::GetIdFromClick(PICKING_RIGHT);

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
			GObject::GetObjectByID(i)->render(false, Picking::leftClickID);
		}
	}
	if (!Minimap::IsActive() && !editor::IsWindowOpened && !editor::menuIsOpened && !editor::TerrainBrushIsActive && Picking::leftClickID_UI == 0 && !editor::movingObject) SelectionRectangle::Render();
}

void Game::GoToPointFromMinimap() {
	if (Engine::Mouse::LeftClick && Engine::Mouse::IsCursorInGameScreen()) {
		cameraToX = Engine::Mouse::GetXLeftClick() / Engine::myWindow::Width*(float)MEDIUM_MAP_WIDTH - Engine::myWindow::WidthZoomed / 2.f;
		cameraToY = Engine::Camera::GetYMinimapCoordinate(Engine::Mouse::GetYLeftClick()) / Engine::myWindow::Height*(float)MEDIUM_MAP_HEIGHT - Engine::myWindow::HeightZoomed / 2.f;
		// if you are clicking on a townhall you have to double click 
		// to move the camera there and quit minimap
		if (Picking::leftClickID > 0 && Picking::hasDoubleClicked()) {
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

void Game::GenerateOutposts(vector<vec2> &locs) {

	string className = "routpost";

	/*for (int i = 0; i < locs.size(); i++) {
		Building* b = new Building();
		b->SetClassName(className);
		b->SetType("building");
		b->SetPlayer(0);
		b->SetPosition(vec3(locs[i].x, locs[i].y, 0.f));
		b->SetPickingID(PickingObject::GetPickingId());
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
						//Building* b = new Building();
						//b->SetClassName(className);
						//b->SetType(type);
						//b->SetPlayer(i);
						//b->SetPosition(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
						//b->SetPickingID(PickingObject::GetPickingId());
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
						d->SetPickingID(PickingObject::GetPickingId());
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
	if (Engine::getEnvironment() == MENU_ENV) return bObjectCreated;

	if (player <= MAX_NUMBER_OF_OBJECTS)
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

