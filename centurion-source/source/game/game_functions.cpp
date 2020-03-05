#include <game/strategy.h>
#include <game/editor.h>
#include <math>
#include <engine/camera.h>
#include <engine/window.h>
#include <engine/mouse.h>
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


bool gameMenuStatus = false;
bool gameMinimapStatus = false;
bool gameGridStatus = false;
int mapWidth = 30000;
int mapHeight = 20000;
//float cameraMovespeed = 10.f;
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

void Strategy::handleKeyboardControls() {
		
	//Open or close minimap
	if (KeyCode[GLFW_KEY_SPACE] || Mouse::MiddleClick) {
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
		Surface::Wireframe = !Surface::Wireframe;
		Surface::Wireframe ? std::cout << "[DEBUG] Wireframe ON!\n" : std::cout << "[DEBUG] Wireframe OFF! \n";
	}
	// Grid
	if (KeyCode[GLFW_KEY_G]) {
		gameGridStatus = !gameGridStatus;
		gameGridStatus ? std::cout << "[DEBUG] Grid ON!\n" : std::cout << "[DEBUG] Grid OFF!\n";
	}
}

// prerendered minimap--------------------

//----------------------------------------

void tracing() {
	if (Mouse::RightClick) {
		unsigned char tracingCol[4];
		Game::Surface::Render(true);
		glReadPixels((GLint)Mouse::GetXRightClick(), (GLint)Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
		mapgen::mouseZNoise = (mapgen::maxZ - mapgen::minZ) * ((float)tracingCol[0] / 255.0f) + mapgen::minZ;
	}
}

/*void renderObjectsPicking() {
		
}*/



void clearBuffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void goToPosition() {
	if (Mouse::LeftClick && cursorInGameScreen()) {
		cameraToX = Mouse::GetXLeftClick() / myWindow::Width*(float)mapWidth - myWindow::WidthZoomed / 2.f;
		cameraToY = getYMinimapCoord(Mouse::GetYLeftClick()) / myWindow::Height*(float)mapHeight - myWindow::HeightZoomed / 2.f;
		// if you are clicking on a townhall you have to double click 
		// to move the camera there and quit minimap
		if (leftClickID > 0 && hasDoubleClicked()) {
			cameraToX = buildings[leftClickID].get_xPos() - myWindow::WidthZoomed / 2.f;
			cameraToY = buildings[leftClickID].get_yPos() - myWindow::HeightZoomed / 2.f;
			blockMinimap = false;
		}
		//------------------------------------------------
		if (!blockMinimap) {
			Camera::go_to_pos(cameraToX, cameraToY);
			gameMinimapStatus = false;
			Mouse::LeftClick = false;
			Mouse::LeftHold = false;
		}
	}
}



void setMinimapProjection() {
	float bottom = (-1.f)*(mapHeight * myWindow::BottomBarHeight / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight));
	float top = mapHeight + mapHeight * myWindow::TopBarHeight / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
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
						buildings[getPickingID()] = b;
							
						if (buildings[getPickingID()].is_independent()) {
							independent_buildings[getPickingID()] = &buildings[getPickingID()];

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
						decorations[getPickingID()] = d;
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
	for (map<int, Building>::iterator bld = buildings.begin(); bld != buildings.end(); bld++) {
		int ID = bld->first;
		if (!bld->second.is_independent()) {
			for (map<int, Building*>::iterator settl = independent_buildings.begin(); settl != independent_buildings.end(); settl++) {
				int settl_ID = settl->first;
				if (settl->second->get_settlement_name() == bld->second.get_settlement_name()) {
					buildings[ID].set_settlement_building(independent_buildings[settl_ID]);
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
		buildings[getPickingID()] = b;
		independent_buildings[getPickingID()] = &buildings[getPickingID()];
		increasePickingID();
	}
}


