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
		blockMinimap = false;
		gameIsCreated = false;
	}

	void Game::reset() {
		units.clear();
		buildings.clear();
		independent_buildings.clear();
		decorations.clear();
		blockMinimap = false;
		gameIsCreated = false;
		gameMenuStatus = false;
		gameMinimapStatus = false;
		gameGridStatus = false;
		blockMinimap = false;
	}

	void Game::create() {
		resetPicking();
		reset();

		setParam("ui-bottom-height", 60.f);
		setParam("ui-top-height", 100.f);
		setMinimapProjection();
		setBoolean("mouse-left-pressed", false);

		selRectangle = gui::Rectangle();
		selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);

		surface = new Surface();
		surface->reset();

		/* GENERATE NOISE */
		surface->createNoise();

		vector<vec2> outpostsLocation;
		vector<vec2> townhallsLocation;


		/* DEFINE SETTLEMENTS POSITIONS */
		mapgen::define_buildings_location(playersNumber, 15, &outpostsLocation);

		/* CREATE SETTLEMENTS */
		generateSettlements(playersNumber);
		generateOutposts(outpostsLocation);


		

		/* UPDATE PASS MATRIX */
		surface->updateGrid();

		/*------------------------------------------------------------*/
		// *********** ROBA PROVVISORIA ***********
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 5; j++){
				Unit u = Unit();
				u.set_class("hmyrmidon");
				u.set_id(getPickingID());
				u.set_player(0);
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
		leftClickID = 0;
		leftClickID_UI = 0;
		resetDoubleClickTime();
	}

	void Game::run() {
		selectedUnits = 0;
		leftClickID_UI = 0;
		CAMERA()->keyboardControl();

		/* Keyboard controls handling*/
		if (!gameMenuStatus) handleKeyboardControls();

		/* If minimap is NOT active */
		if (!gameMinimapStatus) {
			CAMERA()->mouseControl(cameraThreshold);
			view = CAMERA()->calculateViewMatrix();
			projection = glb::cameraProjection;
		}

		/* If minimap is active */
		else {
			view = mat4(1.0f);
			projection = glb::minimapProjection;	
		}

		ui->render(true);

		// apply game matrices:
		obj::applyGameMatrices(&projection, &view);

		/* Tracing and Picking */
		if (!gameMinimapStatus) tracing(surface);
		if(!gameMenuStatus) renderObjectsPicking();
		

		/* Rendering */
		surface->render(false);
		renderObjects();

		// ---- Game UI ---- //

		// apply menu matrices:
		obj::applyMenuMatrices();
		ui->render(false);

		// ----------------- //	

		if(gameMinimapStatus) goToPosition();
		glb::cameraProjection = ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);
		

		// reset mouse-right and mouse-left to improve fps
		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false);
		setBoolean("mouse-middle", false);
	}

	void Game::clear() {
		delete surface;
		delete ui;
	}

	Game::~Game() {}
}