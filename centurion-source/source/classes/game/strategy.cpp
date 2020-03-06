#include <game/strategy.h>
#include <interface>
#include <engine/camera.h>
#include <engine/window.h>
#include <engine/mouse.h>
#include <surface>
#include <player>
#include <picking>

#pragma region Namespaces

using namespace glb;
using namespace std;
using namespace glm;
using namespace engine;
using namespace unit;
using namespace building;

#pragma endregion

#pragma region Static variables

#pragma endregion

Strategy::Strategy() { }

void Strategy::reset() {
	units.clear();
	buildings.clear();
	independent_buildings.clear();
	decorations.clear();
	isCreated = false;
	gameMenuStatus = false;
	gameGridStatus = false;
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
	mapgen::define_buildings_location(playersNumber, 10, &outpostsLocation, &townhallsLocation);

	/* CREATE SETTLEMENTS */
	generateSettlements(townhallsLocation);
	generateOutposts(outpostsLocation);

	/* UPDATE TERRAIN TEXTURES */
	mapgen::defineTerrainZones();


	/* UPDATE PASS MATRIX */
	Surface::UpdateGrid();

	/*------------------------------------------------------------*/
	// *********** ROBA PROVVISORIA ***********
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			Unit u = Unit();
			u.set_class("hmyrmidon");
			u.set_id(getPickingID());
			u.set_player(0);
			u.set_position(playersList[0].getStartPoint().x + i * 50, playersList[0].getStartPoint().y - 1000 - j * 50);
			u.create();
			units[getPickingID()] = u;
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
	selectedUnits = 0;
	leftClickID_UI = 0;
	Camera::keyboardControl();

	/* Keyboard controls handling*/
	if (!gameMenuStatus) handleKeyboardControls();

	/* If minimap is NOT active */
	if (Minimap::IsActive() == false) {
		Camera::mouseControl(cameraThreshold);
		viewMatrix = Camera::calculateViewMatrix();
		projectionMatrix = glb::cameraProjection;

		game::GAME_UI()->render(true);

		// apply game matrices:
		obj::applyGameMatrices(&projectionMatrix, &viewMatrix);

		/* Tracing and Picking */
		Tracing();
		if (!gameMenuStatus) RenderObjectsPicking();

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
		if (!gameMenuStatus) RenderObjectsPicking();

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

	glb::cameraProjection = ortho(0.0f, engine::myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)mapWidth, (float)mapWidth);

	// reset mouse-right and mouse-left to improve fps
	Mouse::RightClick = false;
	Mouse::LeftClick = false;
	Mouse::MiddleClick = false;
}

Strategy::~Strategy() {}
