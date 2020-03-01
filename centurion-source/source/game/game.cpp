#include <game>
#include <interface>
#include <engine/camera.h>
#include <engine/window.h>
#include <engine/mouse.h>
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
		MINIMAP()->setStatus(false);
	}

	void Game::create() {
		resetPicking();
		reset();
		myWindow::BottomBarHeight = 100.f;
		myWindow::TopBarHeight = 100.f;
		setMinimapProjection();
		Mouse::LeftHold = false;

		selRectangle = gui::Rectangle();
		selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);

		surface = new Surface();
		surface->reset();

		/* GENERATE HEIGHT NOISE */
		surface->createNoise();


		/* DEFINE BUILDINGS POSITIONS */
		vector<vec2> outpostsLocation;
		vector<vec2> townhallsLocation;
		mapgen::define_buildings_location(playersNumber, 10, &outpostsLocation, &townhallsLocation);

		/* CREATE SETTLEMENTS */
		generateSettlements(townhallsLocation);
		generateOutposts(outpostsLocation);

		/* UPDATE TERRAIN TEXTURES */
		mapgen::defineTerrainZones();
		

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

		//ui = new UIGame();

		GAME_UI()->create(playersList[0].getPlayerRace().substr(5));

		Camera::go_to_pos(
			(GLfloat)(playersList[0].getStartPoint().x - engine::myWindow::WidthZoomed /2.f),
			(GLfloat)(playersList[0].getStartPoint().y - myWindow::HeightZoomed / 2.f)
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
		Camera::keyboardControl();

		/* Keyboard controls handling*/
		if (!gameMenuStatus) handleKeyboardControls();

		/* If minimap is NOT active */
		if (!gameMinimapStatus) {
			Camera::mouseControl(cameraThreshold);
			view = Camera::calculateViewMatrix();
			projection = glb::cameraProjection;

			GAME_UI()->render(true);

			// apply game matrices:
			obj::applyGameMatrices(&projection, &view);

			/* Tracing and Picking */
			if (!gameMinimapStatus) tracing(surface);
			if (!gameMenuStatus) renderObjectsPicking();

			/* Rendering */
			surface->render(false);
			renderObjects();

			// apply menu matrices:
			obj::applyMenuMatrices();

			GAME_UI()->render(false);
		}


		/* If minimap is active */
		else {
			view = mat4(1.0f);
			projection = glb::minimapProjection;

			GAME_UI()->render(true);

			// apply game matrices:
			obj::applyGameMatrices(&projection, &view);
			if (!gameMenuStatus) renderObjectsPicking();

			if (game::MINIMAP()->getStatus()) game::MINIMAP()->render();

			/* Rendering */
			if (!game::MINIMAP()->getStatus()) {
				surface->render(false);
				game::MINIMAP()->create();
			}
			renderObjects();

			// apply menu matrices:	
			obj::applyMenuMatrices();
			GAME_UI()->render(false);

			if (gameMinimapStatus) goToPosition();
		}
		
		glb::cameraProjection = ortho(0.0f, engine::myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)mapWidth, (float)mapWidth);

		// reset mouse-right and mouse-left to improve fps
		Mouse::RightClick = false;
		Mouse::LeftClick = false;
		Mouse::MiddleClick = false;
	}

	void Game::clear() {
		delete surface;
	}

	Game::~Game() {}
}