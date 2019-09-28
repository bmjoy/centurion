#include "game.h"


Game::Game(){
	
	unit = Unit();

	cameraLastX = 0.0; cameraLastY = 0.0;
	threshold = 20.0f;   // Camera Movement Threshold	
	objectId = 1;
	blockMinimap = false;
	lastTime = glfwGetTime();
	//sel_rect_coords = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	//minimap_rect_coords = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	gameIsCreated = false;
}

void Game::create(std::vector<Player> *ListOfPlayers) {
	surface = new Surface();
	playersList = ListOfPlayers;
	mapgen::setPlayerList(ListOfPlayers);

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	std::cout << "Camera has been created. \n";

	float ui_bottom_height_minimap = GAME::MAP_HEIGHT * GAME::UI_BOTTOM_HEIGHT / (GLB::WINDOW_HEIGHT - GAME::UI_BOTTOM_HEIGHT - GAME::UI_TOP_HEIGHT);
	float ui_top_height_minimap = GAME::MAP_HEIGHT * GAME::UI_TOP_HEIGHT / (GLB::WINDOW_HEIGHT - GAME::UI_BOTTOM_HEIGHT - GAME::UI_TOP_HEIGHT);

	/* MINIMAP CAMERA */
	GLB::MINIMAP_PROJECTION = glm::ortho(
		0.0f,
		(float)GAME::MAP_WIDTH,

		// add and subtract ui height
		ui_bottom_height_minimap * (-1.0f),
		GAME::MAP_HEIGHT + ui_top_height_minimap,

		-(float)GAME::MAP_WIDTH, 
		(float)GAME::MAP_WIDTH
	);

	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/

	/* DEFINE SETTLEMENTS POSITIONS */
	mapgen::define_settlements();

	/* CREATE SETTLEMENTS */
	std::ifstream path("assets/data/settlements.json");
	if (!path.good()) {
		forceGameClosure("Error code 0x00000001\n\nThe game is unable to find or process SETTLEMENTS file.\nForced application shutdown has started.", "Centurion");
	}
	settl_data = json::parse(path);

	for (int i = 0; i < (*playersList).size(); i++) {
		r = (*playersList)[i].getPlayerRace();
		origin = (*playersList)[i].getStartPoint();
		for (int j = 0; j < settl_data[r].size(); j++) {
			b = Building();
			b.set_class(settl_data[r][j]["class"]);
			b.set_id(objectId);
			b.set_player(&(*playersList)[i]);
			b.set_position(glm::vec3(origin.x + (int)settl_data[r][j]["offsetx"], origin.y + (int)settl_data[r][j]["offsety"], 0.0f));
			b.create();
			
			buildingList[objectId] = b;
			objectId++;
		}
	}

	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/
	

	surface->create();
	

	/*------------------------------------------------------------*/

	std::cout << "Terrain has been generated! \n";
	std::cout << "Min(z) = " << MAP::MIN_Z << "; Max(z) = " << MAP::MAX_Z << std::endl;


	// *********** ROBA PROVVISORIA ***********
	unit.set_class("hmyrmidon");
	unit.set_id(objectId);
	unit.set_player(&(*playersList)[0]);
	unit.set_position((*playersList)[0].getStartPoint().x, (*playersList)[0].getStartPoint().y-1000);
	unit.create();
	unitList[objectId] = unit;
	objectId++;

	unit.set_class("hmyrmidon");
	unit.set_id(objectId);
	unit.set_player(&(*playersList)[0]);
	unit.set_position((*playersList)[0].getStartPoint().x + 100, (*playersList)[0].getStartPoint().y - 1000);
	unit.create();
	unitList[objectId] = unit;
	objectId++;

	// ****************************************

	ui.create(&objectId);

	cursor_point.create();
	camera.go_to_pos(
		(GLfloat)((*playersList)[0].getStartPoint().x - GLB::WINDOW_WIDTH_ZOOMED/2.f), 
		(GLfloat)((*playersList)[0].getStartPoint().y - GLB::WINDOW_HEIGHT_ZOOMED / 2.f)
	);


	//---------------------------------------
	gameIsCreated = true;
}

void Game::run() {

	selectedUnits = 0;
	camera.keyboardControl();

	/* If minimap is NOT active */
	if (!GAME::MINIMAP_IS_ACTIVE) {		
		camera.mouseControl(threshold);
		view = camera.calculateViewMatrix();
		projection = GLB::CAMERA_PROJECTION;
	}

	/* If minimap is active */
	else {
		view = glm::mat4(1.0f);
		projection = GLB::MINIMAP_PROJECTION;	
	}

	// apply game matrices:
	obj::applyGameMatrices(&projection, &view);

	/* Tracing and Picking */
	game::tracing(surface, &projection, &view);
	game::picking(&buildingList, &unitList, &projection, &view, &click_id, &blockMinimap);

	/* Rendering */
	surface->render(projection, view, false);
	game::renderObjects(&buildingList, &unitList, &projection, &view, &click_id, &selectedUnits);
	//game::renderSelRectangle(&sel_rect_coords, &view, &cameraLastX, &cameraLastY);
	//game::renderMapRectangle(&minimap_rect_coords);
	
	if (GLB::DEBUG) cursor_point.render();
	
	// ---- Game UI ---- //

	// apply menu matrices:
	obj::applyMenuMatrices();

	ui.render();

	// ----------------- //	

	game::goToPosition(&buildingList, &camera, &lastTime, &click_id, &blockMinimap);
	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, (float)GLB::WINDOW_WIDTH_ZOOMED, 0.0f, (float)GLB::WINDOW_HEIGHT_ZOOMED, -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
}

void Game::clear() {
	delete surface;
}

Game::~Game()
{
}