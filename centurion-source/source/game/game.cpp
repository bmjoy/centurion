#include <game>
#include <engine>
#include <surface>
#include <player>
#include <picking>
#include "../interface/game_ui.h"

using namespace glb;
using namespace std;
using namespace glm;
using namespace unit;
using namespace building;

namespace game {

	Game::Game(){			
		cameraLastX = 0.0; cameraLastY = 0.0;
		threshold = 20.0f;   // Camera Movement Threshold	
		blockMinimap = false;
		lastTime = glfwGetTime();
		gameIsCreated = false;
	}
	void Game::reset() {
		blockMinimap = false;
		gameIsCreated = false;
		gameMenuStatus = false;
		gameMinimapStatus = false;
		gameGridStatus = false;
		clear();
	}
	void Game::create(vector<Player> *ListOfPlayers) {

		resetPicking();
		playersList = ListOfPlayers;
		mapgen::setPlayerList(ListOfPlayers);		
		cout << "DEBUG: Camera has been created.\n";

		camera = new Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

		float ui_bottom_height_minimap = mapHeight * getParam("ui-bottom-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height"));
		float ui_top_height_minimap = mapHeight * getParam("ui-top-height") / (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height"));

		/* MINIMAP CAMERA */
		glb::minimapProjection = ortho(
			0.0f,
			(float)mapWidth,

			// add and subtract ui height
			ui_bottom_height_minimap * (-1.0f),
			mapHeight + ui_top_height_minimap,

			-(float)mapWidth, 
			(float)mapWidth
		);

		selRectangle = gui::Rectangle();
		selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);

		/*------------------------------------------------------------*/
		/*------------------------------------------------------------*/
		/*------------------------------------------------------------*/

		/* DEFINE SETTLEMENTS POSITIONS */
		mapgen::define_settlements();

		/* CREATE SETTLEMENTS */
		ifstream path("assets/data/settlements.json");
		if (!path.good()) {
			forceGameClosure("  Error code 0x00000001\n\n  Unable to find or process SETTLEMENTS file.\nForced application shutdown has started.");
		}
		settl_data = json::parse(path);

		for (int i = 0; i < (*playersList).size(); i++) {
			r = (*playersList)[i].getPlayerRace();
			origin = (*playersList)[i].getStartPoint();
			for (int j = 0; j < settl_data[r].size(); j++) {
				Building b = Building();
				b.set_class(settl_data[r][j]["class"]);
				b.set_id(getPickingID());
				b.set_player(&(*playersList)[i]);
				b.set_position(vec3(origin.x + (int)settl_data[r][j]["offsetx"], origin.y + (int)settl_data[r][j]["offsety"], 0.0f));
				b.create();
			
				buildingList[getPickingID()] = b;
				increasePickingID();
			}
		}

		/*------------------------------------------------------------*/
		/*------------------------------------------------------------*/
		/*------------------------------------------------------------*/
		
		surface = new Surface();
		surface->createNoise();
		surface->updateGrid();
	

		/*------------------------------------------------------------*/

		cout << "DEBUG: Terrain has been generated!\n";
		cout << "DEBUG: Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ << endl;


		// *********** ROBA PROVVISORIA ***********
		Unit u = Unit();
		u.set_class("hmyrmidon");
		u.set_id(getPickingID());
		u.set_player(&(*playersList)[0]);
		u.set_position((*playersList)[0].getStartPoint().x, (*playersList)[0].getStartPoint().y-1000);
		u.create();
		unitList[getPickingID()] = u;
		increasePickingID();;

		u.set_class("hmyrmidon");
		u.set_id(getPickingID());
		u.set_player(&(*playersList)[0]);
		u.set_position((*playersList)[0].getStartPoint().x + 100, (*playersList)[0].getStartPoint().y - 1000);
		u.create();
		unitList[getPickingID()] = u;
		increasePickingID();
		// ****************************************

		ui = new UIGame();
		ui->create();

		camera->go_to_pos(
			(GLfloat)((*playersList)[0].getStartPoint().x - getParam("window-width-zoomed") /2.f),
			(GLfloat)((*playersList)[0].getStartPoint().y - getParam("window-height-zoomed") / 2.f)
		);


		//---------------------------------------
		gameIsCreated = true;
	}

	void Game::run() {
		selectedUnits = 0;
		camera->keyboardControl();

		/* If minimap is NOT active */
		if (!gameMinimapStatus) {		
			camera->mouseControl(threshold);
			view = camera->calculateViewMatrix();
			projection = glb::cameraProjection;
		}

		/* If minimap is active */
		else {
			view = mat4(1.0f);
			projection = glb::minimapProjection;	
		}

		// apply game matrices:
		obj::applyGameMatrices(&projection, &view);

		/* Tracing and Picking */
		tracing(surface, &projection, &view);
		picking(&buildingList, &unitList, &projection, &view, &click_id, &blockMinimap);

		/* Rendering */
		surface->render(false);
		renderObjects(&buildingList, &unitList, &selRectangle, &projection, &view, &click_id, &selectedUnits);
	
	
	
		// ---- Game UI ---- //

		// apply menu matrices:
		obj::applyMenuMatrices();

		ui->render();

		// ----------------- //	

		goToPosition(&buildingList, camera, &lastTime, &click_id, &blockMinimap);
		glb::cameraProjection = ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);
		
		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false); // temporary
	}

	void Game::clear() {
		delete surface;
		delete ui;
		delete camera;
	}

	Game::~Game()
	{		
	}
}