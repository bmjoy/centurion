#include <game>
#include <editor>
#include <math>
#include <engine>
#include <player>
#include <surface>
#include <picking>
#include <interface>

using namespace glb;
using namespace std;
using namespace glm;
using namespace unit;
using namespace engine;
using namespace building;
using namespace decoration;

namespace game {

	Game *GAME() { return &mygame; }
	Minimap *MINIMAP() { return &myminimap; }

	bool gameMenuStatus = false;
	bool gameMinimapStatus = false;
	bool gameGridStatus = false;
	int mapWidth = 30000;
	int mapHeight = 20000;
	float cameraMovespeed = 10.f;
	int playersNumber = 1;
	int maxPlayersNumber = 10;
	int currentZoomCamera = 8;
	float zoomCameraFactor = 100.f;
	float townhallRadius = 1875.f;
	float outpostRadius = 700.f;
	bool selRectangleIsActive = false;
	bool blockMinimap = false;
	int selectedUnits = 0;

	map<int, Building> buildings = { };
	map<int, Building*> independent_buildings = { };
	map<int, Unit> units = { };
	map<int, Decoration> decorations = { };

	vector<string> outposts;

	array<Player, 8> playersList;

	SelRectPoints *SelRectCoords() { return &selRectCoords; }

	void Game::handleKeyboardControls() {
		
		//Open or close minimap
		if (KeyCode[GLFW_KEY_SPACE] || getBoolean("mouse-middle")) {
			gameMinimapStatus = !gameMinimapStatus;
			gameMinimapStatus ? std::cout << "[DEBUG] Minimap camera ON!\n" : std::cout << "[DEBUG] Minimap camera OFF!\n";
		}
		//Open in-game menu
		if (KeyCode[GLFW_KEY_ESCAPE]) {
			gameMenuStatus = !gameMenuStatus;
			gameMinimapStatus ? std::cout << "[DEBUG] Pause Menu ON!\n" : std::cout << "[DEBUG] Pause Menu OFF!\n";
		}
		// Wireframe
		if (KeyCode[GLFW_KEY_Z]) {
			setBoolean("wireframe", !getBoolean("wireframe"));
			getBoolean("wireframe") ? std::cout << "[DEBUG] Wireframe ON!\n" : std::cout << "[DEBUG] Wireframe OFF! \n";
		}
		// Grid
		if (KeyCode[GLFW_KEY_G]) {
			gameGridStatus = !gameGridStatus;
			gameGridStatus ? std::cout << "[DEBUG] Grid ON!\n" : std::cout << "[DEBUG] Grid OFF!\n";
		}
	}

	// prerendered minimap--------------------
	Minimap::Minimap() {
		isCreated = false;
	}
	void Minimap::create() {
		obj::MMRectangle()->update();
		isCreated = true;
	}
	void Minimap::render() {
		obj::MMRectangle()->render();
	}
	Minimap::~Minimap(){}
	//----------------------------------------

	void tracing(Surface *s) {
		if (getBoolean("mouse-right")) {
			unsigned char tracingCol[4];
			(*s).render(true);
			glReadPixels((GLint)getParam("mouse-x-rightclick"), (GLint)getParam("mouse-y-rightclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
			mapgen::mouseZNoise = (mapgen::maxZ - mapgen::minZ) * ((float)tracingCol[0] / 255.0f) + mapgen::minZ;
		}
	}

	void renderObjectsPicking() {
		if (leftClickID_UI != 0) {
			leftClickID = 0;
			rightClickID = 0;
			return;
		}

		if ((getBoolean("mouse-right") || getBoolean("mouse-left")) && !selRectangleIsActive){
			for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
				bld->second.render(true, 0);
			}
			for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(true, 0);
			}	

			if (getBoolean("mouse-left")) leftClickID = get_id("left");
			if (getBoolean("mouse-right")) rightClickID = get_id("right");

			if (gameMinimapStatus) {					
				blockMinimap = false;
				if (leftClickID > 0) {
					blockMinimap = true;
				}
			}
		}
	}

	void renderObjects() {
		int selectedBuildings = 0;
		for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
			bld->second.render(false, leftClickID);
			if (bld->second.isSelected()) selectedBuildings++;
		}
		if (selectedBuildings == 0) game::GAME_UI()->set_ui(nullptr);

		for (map<int, Decoration>::iterator dec = decorations.begin(); dec != decorations.end(); dec++) {
			dec->second.render();
		}
		if (!gameMinimapStatus) {
			for (map<int, Unit>::iterator u = units.begin(); u != units.end(); u++) {
				u->second.render(false, leftClickID);
				if (u->second.isSelected())	  selectedUnits++;
			}
		}
		/*cout << gameMinimapStatus << " " << editor::IsWindowOpened << " " << editor::menuIsOpened << " " << editor::TerrainBrushIsActive << " " << leftClickID_UI << editor::movingObject << endl;*/
		if (!gameMinimapStatus && !editor::IsWindowOpened && !editor::menuIsOpened && !editor::TerrainBrushIsActive && leftClickID_UI == 0 && !editor::movingObject) renderSelRectangle();
	}

	void clearBuffers() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void goToPosition() {
		if (getBoolean("mouse-left") && cursorInGameScreen()) {
			cameraToX = getParam("mouse-x-leftclick") / getParam("window-width")*(float)mapWidth - getParam("window-width-zoomed") / 2.f;
			cameraToY = getYMinimapCoord(getParam("mouse-y-leftclick")) / getParam("window-height")*(float)mapHeight - getParam("window-height-zoomed") / 2.f;
			// if you are clicking on a townhall you have to double click 
			// to move the camera there and quit minimap
			if (leftClickID > 0 && hasDoubleClicked()) {
				cameraToX = buildings[leftClickID].get_xPos() - getParam("window-width-zoomed") / 2.f;
				cameraToY = buildings[leftClickID].get_yPos() - getParam("window-height-zoomed") / 2.f;
				blockMinimap = false;
			}
			//------------------------------------------------
			if (!blockMinimap) {
				CAMERA()->go_to_pos(cameraToX, cameraToY);
				gameMinimapStatus = false;
				setBoolean("mouse-left", false);
				setBoolean("mouse-left-pressed", false);
			}
		}
	}

	void renderSelRectangle() {
		if (getBoolean("mouse-left-pressed")) {
			if (!selRectangleIsActive){
				cout << "[DEBUG] Selection rectangle enabled.\n";
				(*SelRectCoords()).startX = getParam("mouse-x-leftclick") * getParam("window-width-zoomed") / getParam("window-width") + cameraLastX;
				(*SelRectCoords()).startY = getParam("mouse-y-leftclick") * getParam("window-height-zoomed") / getParam("window-height") + cameraLastY;
			}
			(*SelRectCoords()).lastX = getParam("mouse-x-position") * getParam("window-width-zoomed") / getParam("window-width") + getParam("camera-x-position");
			(*SelRectCoords()).lastY = getParam("mouse-y-position") * getParam("window-height-zoomed") / getParam("window-height") + getParam("camera-y-position");
			if (getParam("mouse-y-position") < getParam("ui-bottom-height")) {
				(*SelRectCoords()).lastY = getParam("ui-bottom-height")*getParam("window-height-zoomed") / getParam("window-height") + 1.0f + getParam("camera-y-position");
			}
			if (getParam("mouse-y-position") > getParam("window-height") - getParam("ui-top-height")) {
				(*SelRectCoords()).lastY = getParam("window-height-zoomed") - getParam("ui-top-height")*getParam("window-height-zoomed") / getParam("window-height") - 1.0f + getParam("camera-y-position");
			}

			float w = ((*SelRectCoords()).lastX - (*SelRectCoords()).startX);
			float h = ((*SelRectCoords()).lastY - (*SelRectCoords()).startY);

			int origin = 0;
			if (w > 0 && h > 0) origin = 0; // bottom-left
			if (w > 0 && h < 0) origin = 1; // top-left
			if (w < 0 && h > 0) origin = 4; // bottom-right
			if (w < 0 && h < 0) origin = 3; // top-right

			if (abs(w) > 1 && abs(h) > 1){
				GAME()->selRectangle.render(vec4(255.f), 0, (*SelRectCoords()).startX, (*SelRectCoords()).startY, abs(w), abs(h), origin);
				(*SelRectCoords()).minX = std::min((*SelRectCoords()).startX, (*SelRectCoords()).lastX);
				(*SelRectCoords()).maxX = std::max((*SelRectCoords()).startX, (*SelRectCoords()).lastX);
				(*SelRectCoords()).minY = std::min((*SelRectCoords()).startY, (*SelRectCoords()).lastY);
				(*SelRectCoords()).maxY = std::max((*SelRectCoords()).startY, (*SelRectCoords()).lastY);
			}
			else {
				(*SelRectCoords()).minX = -0.1f;
				(*SelRectCoords()).maxX = -0.1f;
				(*SelRectCoords()).minY = -0.1f;
				(*SelRectCoords()).maxY = -0.1f;
			}	
			selRectangleIsActive = true;
		}
		else {
			if (selRectangleIsActive) cout << "[DEBUG] Selection rectangle disabled.\n";
			cameraLastX = getParam("camera-x-position");
			cameraLastY = getParam("camera-y-position");
			(*SelRectCoords()).startX = -0.1f;
			(*SelRectCoords()).startY = -0.1f;
			(*SelRectCoords()).lastX = -0.1f;
			(*SelRectCoords()).lastY = -0.1f;
			(*SelRectCoords()).minX = -0.1f;
			(*SelRectCoords()).maxX = -0.1f;
			(*SelRectCoords()).minY = -0.1f;
			(*SelRectCoords()).maxY = -0.1f;
			selRectangleIsActive = false;
		}
	}

	void setMinimapProjection() {
		float bottom = (-1.f)*(mapHeight * getParam("ui-bottom-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")));
		float top = mapHeight + mapHeight * getParam("ui-top-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height"));
		float left = 0.f;
		float right = (float)mapWidth;
		glb::minimapProjection = ortho(left, right, bottom, top, -right, right);
	}

	void generateSettlements(vector<vec2> &locs) {
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
							building::Building b = building::Building();
							b.set_class(className);
							b.set_player(i);
							b.set_position(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
							b.set_id(getPickingID());
							b.set_settlement_name("Settlement_player_"+i);
							b.create();
							game::buildings[getPickingID()] = b;
							
							if (game::buildings[getPickingID()].is_independent()) {
								game::independent_buildings[getPickingID()] = &game::buildings[getPickingID()];

								// update terrain around the townhall
								// N.B: mapgen::grid_size * 2 because the map has "borders"
								
								int EditorStartPointX = (int)xEditorPos - (int)townhallRadius + mapgen::grid_size * 2;
								int EditorStartPointY = (int)yEditorPos - (int)townhallRadius + mapgen::grid_size * 2;
								int NewMapStartPointX = (int)origin.x - (int)townhallRadius + mapgen::grid_size * 2;
								int NewMapStartPointY = (int)origin.y - (int)townhallRadius + mapgen::grid_size * 2;

								for (int iHoriz = 0; iHoriz <= townhallRadius * 2; iHoriz += mapgen::grid_size) {
									for (int iVert = 0; iVert <= townhallRadius * 2; iVert += mapgen::grid_size) {

										int EditorPointX = EditorStartPointX + iHoriz; 
										int EditorPointY = EditorStartPointY + iVert;
										int NewMapPointX = NewMapStartPointX + iHoriz;
										int NewMapPointY = NewMapStartPointY + iVert;

										if (math::euclidean_distance((float)NewMapPointX, (float)NewMapPointY, origin.x + mapgen::grid_size * 2.f, origin.y + mapgen::grid_size * 2.f) > townhallRadius) continue;

										int EditorPointLoc = mapgen::getVertexPos(EditorPointX, EditorPointY);
										int NewMapPointLoc = mapgen::getVertexPos(NewMapPointX, NewMapPointY);
										mapgen::MapTextures()[NewMapPointLoc] = float(TemporaryMapTextures[EditorPointLoc]);
									}
								}
							}
							increasePickingID();
						}
						if (type == "decoration") {
							decoration::Decoration d = decoration::Decoration();
							d.set_class(className);
							d.set_player(0);
							d.set_position(vec3(origin.x + xOffset, origin.y + yOffset, 0.f));
							d.set_id(getPickingID());
							d.create();
							game::decorations[getPickingID()] = d;
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
		for (map<int, Building>::iterator bld = game::buildings.begin(); bld != game::buildings.end(); bld++) {
			int ID = bld->first;
			if (!bld->second.is_independent()) {
				for (map<int, Building*>::iterator settl = game::independent_buildings.begin(); settl != game::independent_buildings.end(); settl++) {
					int settl_ID = settl->first;
					if (settl->second->get_settlement_name() == bld->second.get_settlement_name()) {
						game::buildings[ID].set_settlement_building(game::independent_buildings[settl_ID]);
						break;
					}
				}
			}
		}
	}


	void generateOutposts(vector<vec2> &locs) {

		string className = "routpost";

		for (int i = 0; i < locs.size(); i++) {
			building::Building b = building::Building();
			b.set_class(className);
			b.set_player(0);
			b.set_position(vec3(locs[i].x, locs[i].y, 0.f));
			b.set_id(getPickingID());
			b.set_settlement_name("Outpost_" + i);
			b.create();
			game::buildings[getPickingID()] = b;
			game::independent_buildings[getPickingID()] = &game::buildings[getPickingID()];
			increasePickingID();
		}
	}
}

