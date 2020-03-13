#include <game/strategy.h>
#include <interface>
#include <engine.h>
#include <surface>
#include <player>
#include <picking.h>
#include <object/unit.h>

#include <global>

#pragma region Namespaces

using namespace glb;
using namespace std;
using namespace glm;

#pragma endregion

#pragma region Static variables

#pragma endregion

Strategy::Strategy() { }

void Strategy::reset() {

	ResetGameObjects();

	isCreated = false;
	game::GameMenu::Disable();
	Surface::DisableGrid();
	Minimap::Unblock();
	Minimap::Disable();
	Minimap::Update();
}

void Strategy::Create() {

	PickingObject::resetPicking();
	PickingUI::resetPicking();

	reset();
	Engine::myWindow::BottomBarHeight = 100.f;
	Engine::myWindow::TopBarHeight = 100.f;

	setMinimapProjectionMatrix();

	Engine::Mouse::LeftHold = false;

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
			u->SetClassName("hmyrmidon");
			u->SetPickingID(PickingObject::GetPickingId());
			u->SetPlayer(0);
			u->set_position(playersList[0].getStartPoint().x + i * 50, playersList[0].getStartPoint().y - 1000 - j * 50);
			u->create();
			GameObjects[u->GetPickingID()] = u;
			//units[u->GetPickingID()] = u;
		}
	}

	// ****************************************

	//ui = new UIGame();

	game::GAME_UI()->create(playersList[0].getPlayerRace().substr(5));

	Engine::Camera::GoToPoint(
		(GLfloat)(playersList[0].getStartPoint().x - Engine::myWindow::WidthZoomed / 2.f),
		(GLfloat)(playersList[0].getStartPoint().y - Engine::myWindow::HeightZoomed / 2.f)
	);

	//---------------------------------------
	isCreated = true;
	Picking::leftClickID = 0;
	Picking::leftClickID_UI = 0;
	Picking::resetDoubleClickTime();
}

void Strategy::Run() {
	Unit::ResetCounter();
	Picking::leftClickID_UI = 0;
	Engine::Camera::keyboardControl();

	/* Keyboard controls handling*/
	if (!game::GameMenu::IsActive()) handleKeyboardControls();

	/* If minimap is NOT active */
	if (Minimap::IsActive() == false) {

		Engine::Camera::mouseControl();
		viewMatrix = Engine::Camera::calculateViewMatrix();
		projectionMatrix = getCameraProjectionMatrix();

		game::GAME_UI()->render(true);

		// apply game matrices:
		applyGameMatrices(&projectionMatrix, &viewMatrix);

		/* Tracing and Picking */
		Tracing();
		if (!game::GameMenu::IsActive()) RenderObjectsPicking();

		/* Rendering */
		Surface::Render(false);
		RenderObjects();

		// apply menu matrices:
		applyMenuMatrices();

		game::GAME_UI()->render(false);

	}


	/* If minimap is active */
	else {
		viewMatrix = mat4(1.0f);
		projectionMatrix = getMinimapProjectionMatrix();

		game::GAME_UI()->render(true);

		// apply game matrices:
		applyGameMatrices(&projectionMatrix, &viewMatrix);
		if (!game::GameMenu::IsActive()) RenderObjectsPicking();

		if (Minimap::IsCreated()) Minimap::Render();

		/* Rendering */
		if (!Minimap::IsCreated()) {
			Surface::Render(false);
			Minimap::Create();
		}
		RenderObjects();

		// apply menu matrices:	
		applyMenuMatrices();
		game::GAME_UI()->render(false);

		if (Minimap::IsActive()) GoToPointFromMinimap();
	}

	setCameraProjectionMatrix(ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));

	// reset mouse-right and mouse-left to improve fps
	Engine::Mouse::RightClick = false;
	Engine::Mouse::LeftClick = false;
	Engine::Mouse::MiddleClick = false;
}

void Strategy::handleKeyboardControls() {

	//Open or close minimap
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_SPACE) || Engine::Mouse::MiddleClick) {
		if (Minimap::IsActive()) Minimap::Disable();
		else Minimap::Enable();
		Minimap::IsActive() ? Logger::Info("Minimap camera ON!") : Logger::Info("Minimap camera OFF!");
	}
	//Open in-game menu
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		if (game::GameMenu::IsActive()) game::GameMenu::Disable();
		else game::GameMenu::Enable();
		game::GameMenu::IsActive() ? Logger::Info("Pause Menu ON!") : Logger::Info("Pause Menu OFF!");
	}
	// Wireframe
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_Z)) {
		Surface::Wireframe = !Surface::Wireframe;
		Surface::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
	}
	// Grid
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_G)) {
		if (Surface::IsGridEnabled()) Surface::DisableGrid();
		else Surface::EnableGrid();
		Surface::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
	}
}

Strategy::~Strategy() {}
