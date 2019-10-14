#include <game>
#include <interface>
#include <engine>
#include <surface>
#include <player>
#include <picking>

using namespace glb;
using namespace std;
using namespace glm;
using namespace engine;
using namespace unit;
using namespace building;

namespace game {

	Game::Game(){			
		cameraLastX = 0.0; cameraLastY = 0.0;
		threshold = 20.0f;   // Camera Movement Threshold	
		blockMinimap = false;
		gameIsCreated = false;
		click_id = 0;
	}
	void Game::reset() {
		units = { };
		selectedUnits = { };
		buildings = { };
		blockMinimap = false;
		gameIsCreated = false;
		gameMenuStatus = false;
		gameMinimapStatus = false;
		gameGridStatus = false;
		clear();
	}
	void Game::create() {
		
		resetPicking();
		units = { };
		buildings = { };	
		
		setMinimapProjection();
		setBoolean("mouse-left-pressed", false);

		selRectangle = gui::Rectangle();
		selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);

		/*------------------------------------------------------------*/
		/* DEFINE SETTLEMENTS POSITIONS */
		mapgen::define_settlements();

		/* CREATE SETTLEMENTS */
		//Close the game if it wasn't able to find or process settlements.json file
		ifstream path("assets/data/settlements.json");
		if (!path.good()) {
			forceGameClosure("NOT_FOUND", "ERROR_settlements");
		}
		settl_data = json::parse(path);

		for (int i = 0; i < playersList.size(); i++) {
			r = playersList[i].getPlayerRace();
			origin = playersList[i].getStartPoint();
			for (int j = 0; j < settl_data[r].size(); j++) {
				Building b = Building();
				b.set_class(settl_data[r][j]["class"]);
				b.set_id(getPickingID());
				b.set_player(&playersList[i]);
				b.set_position(vec3(origin.x + (int)settl_data[r][j]["offsetx"], origin.y + (int)settl_data[r][j]["offsety"], 0.0f));
				b.create();
			
				buildings[getPickingID()] = b;
				increasePickingID();
			}
		}
		/*------------------------------------------------------------*/
		
		surface = new Surface();
		surface->createNoise();
		surface->updateGrid();

		/*------------------------------------------------------------*/
		// *********** ROBA PROVVISORIA ***********
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				Unit u = Unit();
				u.set_class("hmyrmidon");
				u.set_id(getPickingID());
				u.set_player(&playersList[0]);
				u.set_position(playersList[0].getStartPoint().x+i*50, playersList[0].getStartPoint().y-1000-j*50);
				u.create();
				units[getPickingID()] = u;
				increasePickingID();
			}
		}
		// ****************************************

		ui = new UIGame();
		ui->create();

		CAMERA()->go_to_pos(
			(GLfloat)(playersList[0].getStartPoint().x - getParam("window-width-zoomed") /2.f),
			(GLfloat)(playersList[0].getStartPoint().y - getParam("window-height-zoomed") / 2.f)
		);

		//---------------------------------------
		gameIsCreated = true;
		lastTime = glfwGetTime();
	}

	void Game::run() {
		selectedUnits = 0;
		CAMERA()->keyboardControl();

		/* If minimap is NOT active */
		if (!gameMinimapStatus) {		
			CAMERA()->mouseControl(threshold);
			view = CAMERA()->calculateViewMatrix();
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
		tracing(surface);
		picking(&click_id, &blockMinimap);

		/* Rendering */
		surface->render(false);
		renderObjects(click_id);

		// ---- Game UI ---- //

		// apply menu matrices:
		obj::applyMenuMatrices();
		ui->render();

		// ----------------- //	

		goToPosition(&lastTime, click_id, &blockMinimap);
		glb::cameraProjection = ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);
		

		// reset mouse-right and mouse-left to improve fps
		// excluding some cases

		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false);
	}

	void Game::clear() {
		delete surface;
		delete ui;
	}

	Game::~Game()
	{		
	}
}