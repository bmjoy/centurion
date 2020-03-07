#include <game/strategy.h>
#include <interface>
#include <engine/camera.h>
#include <engine/window.h>
#include <engine/mouse.h>
#include <surface>
#include <player>
#include <picking>
#include <object/unit.h>

#pragma region Namespaces

using namespace glb;
using namespace std;
using namespace glm;
using namespace engine;



#pragma endregion

#pragma region Static variables

#pragma endregion

Strategy::Strategy() { }

void Strategy::reset() {
	//units.clear();
	//buildings.clear();
	//independent_buildings.clear();
	//decorations.clear();
	
	ResetGameObjects();

	isCreated = false;
	game::GameMenu::Disable();
	Surface::DisableGrid();
	Minimap::Unblock();
	Minimap::Disable();
	Minimap::Update();
}

void Strategy::Create() {
	resetPicking();
	resetPicking_UI();
	reset();
	myWindow::BottomBarHeight = 100.f;
	myWindow::TopBarHeight = 100.f;
	setMinimapProjection();
	Mouse::LeftHold = false;

	SelectionRectangle::Create();

	Surface::Reset();

	/* GENERATE HEIGHT NOISE */
	Surface::CreateNoise();


	/* DEFINE BUILDINGS POSITIONS */
	vector<vec2> outpostsLocation;
	vector<vec2> townhallsLocation;
	mapgen::define_buildings_location(numberOfPlayers, 10, &outpostsLocation, &townhallsLocation);

	/* CREATE SETTLEMENTS */
	Game::GenerateSettlements(townhallsLocation);
	Game::GenerateOutposts(outpostsLocation);

	/* UPDATE TERRAIN TEXTURES */
	mapgen::defineTerrainZones();


	/* UPDATE PASS MATRIX */
	Surface::UpdateGrid();

	/*------------------------------------------------------------*/
	// *********** ROBA PROVVISORIA ***********
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Unit* u = new Unit();
			u->set_class("hmyrmidon");
			u->set_id(getPickingID());
			u->set_player(0);
			u->set_position(playersList[0].getStartPoint().x + i * 50, playersList[0].getStartPoint().y - 1000 - j * 50);
			u->create();
			GameObjects[getPickingID()] = u;
			//units[getPickingID()] = u;
			increasePickingID();
		}
	}

	// ****************************************

	//ui = new UIGame();

	game::GAME_UI()->create(playersList[0].getPlayerRace().substr(5));

	Camera::GoToPoint(
		(GLfloat)(playersList[0].getStartPoint().x - myWindow::WidthZoomed / 2.f),
		(GLfloat)(playersList[0].getStartPoint().y - myWindow::HeightZoomed / 2.f)
	);

	//---------------------------------------
	isCreated = true;
	leftClickID = 0;
	leftClickID_UI = 0;
	resetDoubleClickTime();
}

void Strategy::Run() {
	Unit::ResetCounter();
	leftClickID_UI = 0;
	Camera::keyboardControl();

	/* Keyboard controls handling*/
	if (!game::GameMenu::IsActive()) handleKeyboardControls();

	/* If minimap is NOT active */
	if (Minimap::IsActive() == false) {

		Camera::mouseControl();
		viewMatrix = Camera::calculateViewMatrix();
		projectionMatrix = glb::cameraProjection;

		game::GAME_UI()->render(true);

		// apply game matrices:
		obj::applyGameMatrices(&projectionMatrix, &viewMatrix);

		/* Tracing and Picking */
		Tracing();
		if (!game::GameMenu::IsActive()) RenderObjectsPicking();

		/* Rendering */
		Surface::Render(false);
		RenderObjects();

		// apply menu matrices:
		obj::applyMenuMatrices();

		game::GAME_UI()->render(false);

	}


	/* If minimap is active */
	else {
		viewMatrix = mat4(1.0f);
		projectionMatrix = glb::minimapProjection;

		game::GAME_UI()->render(true);

		// apply game matrices:
		obj::applyGameMatrices(&projectionMatrix, &viewMatrix);
		if (!game::GameMenu::IsActive()) RenderObjectsPicking();

		if (Minimap::IsCreated()) Minimap::Render();

		/* Rendering */
		if (!Minimap::IsCreated()) {
			Surface::Render(false);
			Minimap::Create();
		}
		RenderObjects();

		// apply menu matrices:	
		obj::applyMenuMatrices();
		game::GAME_UI()->render(false);

		if (Minimap::IsActive()) GoToPointFromMinimap();
	}

	glb::cameraProjection = ortho(0.0f, engine::myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH);

	// reset mouse-right and mouse-left to improve fps
	Mouse::RightClick = false;
	Mouse::LeftClick = false;
	Mouse::MiddleClick = false;
}

void Strategy::handleKeyboardControls() {

	//Open or close minimap
	if (KeyCode[GLFW_KEY_SPACE] || Mouse::MiddleClick) {
		if (Minimap::IsActive()) Minimap::Disable();
		else Minimap::Enable();
		Minimap::IsActive() ? std::cout << "[DEBUG] Minimap camera ON!\n" : std::cout << "[DEBUG] Minimap camera OFF!\n";
	}
	//Open in-game menu
	if (KeyCode[GLFW_KEY_ESCAPE]) {
		if (game::GameMenu::IsActive()) game::GameMenu::Disable();
		else game::GameMenu::Enable();
		game::GameMenu::IsActive() ? std::cout << "[DEBUG] Pause Menu ON!\n" : std::cout << "[DEBUG] Pause Menu OFF!\n";
	}
	// Wireframe
	if (KeyCode[GLFW_KEY_Z]) {
		Surface::Wireframe = !Surface::Wireframe;
		Surface::Wireframe ? std::cout << "[DEBUG] Wireframe ON!\n" : std::cout << "[DEBUG] Wireframe OFF! \n";
	}
	// Grid
	if (KeyCode[GLFW_KEY_G]) {
		if (Surface::IsGridEnabled()) Surface::DisableGrid();
		else Surface::EnableGrid();
		Surface::IsGridEnabled() ? std::cout << "[DEBUG] Grid ON!\n" : std::cout << "[DEBUG] Grid OFF!\n";
	}
}

Strategy::~Strategy() {}
