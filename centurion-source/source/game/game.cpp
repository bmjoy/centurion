#include "game.h"


Game::Game(){
	threshold = 20.0f;   // Camera Movement Threshold	

	cameraLastX = 0.0; cameraLastY = 0.0;
	camera_to_x = 0.0f; camera_to_y = 0.0f;

	surface = Surface();

	minimapRectangle = EmptyRectangle();
	selectionRectangle = EmptyRectangle();

	unit = Unit();

	objectId = 1;

	clicks_count = 0;
	blockMinimap = false;
	doubleClick = false;
	lastTime = glfwGetTime();

	sel_rect_coords = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	minimap_rect_coords = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

}

void Game::create() {	
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
	GAME::TOWNHALL_POS = define_settlements(GAME::PLAYERS_NUMBER);

	/* CREATE SETTLEMENTS */
	std::ifstream path("assets/data/settlements.json");
	if (!path.good()) {
		forceGameClosure("Error code 0x00000001\n\nThe game is unable to find or process SETTLEMENTS file.\nForced application shutdown has started.", "Imper4m");
	}
	settl_data = json::parse(path);

	for (int i = 0; i < GAME::PLAYERS_NUMBER; i++) {
		r = GAME::PLAYERS_RACE[i];
		origin = glm::vec2(GAME::TOWNHALL_POS[i * 2], GAME::TOWNHALL_POS[i * 2 + 1]);
		for (int j = 0; j < settl_data[r].size(); j++) {
			b = Building();
			b.set_class(settl_data[r][j]["class"]);
			b.set_id(objectId);
			b.set_player(i);

			b.set_position(glm::vec3(origin.x + (int)settl_data[r][j]["offsetx"], origin.y + (int)settl_data[r][j]["offsety"], 0.0f));
			b.create();
			b.create_pass();
			
			buildingList[objectId] = b;
			objectId++;
		}
	}

	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/
	/*------------------------------------------------------------*/
	

	surface.create();
	

	/*------------------------------------------------------------*/

	std::cout << "Terrain has been generated! \n";
	std::cout << "Min(z) = " << MAP::MIN_Z << "; Max(z) = " << MAP::MAX_Z << std::endl;

	minimapRectangle.compile();
	minimapRectangle.init();
	minimapRectangle.apply_projection_matrix(GLB::MINIMAP_PROJECTION);

	selectionRectangle.compile(); 
	selectionRectangle.init();
	selectionRectangle.apply_projection_matrix(GLB::CAMERA_PROJECTION);

	unit.set_class("hmyrmidon");
	unit.set_player(0);
	unit.set_position(GAME::TOWNHALL_POS[0], GAME::TOWNHALL_POS[1] - 1000.f);
	unit.create();

	std::cout << "Unit is ready to fight. \n";

	ui.create();

	cursor_point.create();
}

void Game::run() {
	/* If minimap is NOT active */

	if (!GAME::MINIMAP_IS_ACTIVE) camera.mouseControl(threshold);

	if (!GAME::MINIMAP_IS_ACTIVE) {		
		view = camera.calculateViewMatrix();
		projection = GLB::CAMERA_PROJECTION;			
	}

	/* If minimap is active */

	else {
		view = glm::mat4(1.0f);
		projection = GLB::MINIMAP_PROJECTION;			
	}

	camera.keyboardControl();
	/* Tracing and Picking */

	tracing();	
	picking();

	/* Rendering */

	surface.render(projection, view, false);

	if (!GAME::GRID_IS_ACTIVE){
		for (std::map<int, Building>::iterator bld = buildingList.begin(); bld != buildingList.end(); bld++) {
			bld->second.select(bld->second.picking_id == click_id);
			bld->second.render(projection, view, false);
		}
	}

	unit.render(view);

	render_selection_rectangle();
	render_minimap_rectangle();

	go_to_position();

	cursor_point.render();
	ui.render();

	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, (float)GLB::WINDOW_WIDTH_ZOOMED, 0.0f, (float)GLB::WINDOW_HEIGHT_ZOOMED, -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
}

float yMinimapCoord(float x) {
	return (float)GLB::WINDOW_HEIGHT * (x - GAME::UI_BOTTOM_HEIGHT) / ((float)GLB::WINDOW_HEIGHT - GAME::UI_BOTTOM_HEIGHT - GAME::UI_TOP_HEIGHT);
}

void Game::tracing() {
	if (!GAME::MINIMAP_IS_ACTIVE) {
		unsigned char tracingCol[4];
		surface.render(projection, view, true);
		glReadPixels(GLB::MOUSE_LEFT_X, GLB::MOUSE_LEFT_Y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &tracingCol);
		GLB::Z_NOISE = (MAP::MAX_Z - MAP::MIN_Z) * ((float)tracingCol[0] / 255.0) + MAP::MIN_Z;
		clear_buffers();
	}
}


void Game::picking() {
	for (std::map<int, Building>::iterator bld = buildingList.begin(); bld != buildingList.end(); bld++) {
		bld->second.render(projection, view, true);
	}
	if (GLB::MOUSE_LEFT) {
		click_id = get_id();

		if (GAME::MINIMAP_IS_ACTIVE) {
			blockMinimap = false;
			if (click_id > 0) { 
				blockMinimap = true; 
			}
		}
	}
}

void Game::go_to_position() {
	if (GAME::MINIMAP_IS_ACTIVE) {
		if (GLB::MOUSE_LEFT && cursorInGameScreen()) {

			camera_to_x = GLB::MOUSE_LEFT_X / (float)GLB::WINDOW_WIDTH*(float)GAME::MAP_WIDTH - (float)GLB::WINDOW_WIDTH / 2.0;
			camera_to_y = yMinimapCoord((float)GLB::MOUSE_LEFT_Y) / (float)GLB::WINDOW_HEIGHT*(float)GAME::MAP_HEIGHT - (float)GLB::WINDOW_HEIGHT / 2.0;

			/* Double Click detection */
			// if you are clicking on a townhall you have to double click 
			// to move the camera there and quit minimap
			
			if (glfwGetTime() - lastTime > 1.0f) { clicks_count = 0; }

			if (clicks_count == 0) {
				clicks_count++;
				clickIdList[0] = click_id;
				lastTime = glfwGetTime();
				doubleClick = false;
			}
			else if (clicks_count == 1) {
				if (glfwGetTime() - lastTime < 1.0f) {
					clickIdList[1] = click_id;
					if (clickIdList[1] == clickIdList[0]) {
						doubleClick = true;
					}
					else {
						clickIdList[0] = click_id;
					}
				}
			}

			if (doubleClick && click_id > 0) {
				camera_to_x = buildingList[click_id].get_xPos()-GLB::WINDOW_WIDTH_ZOOMED/2;
				camera_to_y = buildingList[click_id].get_yPos()-GLB::WINDOW_HEIGHT_ZOOMED/2;
				blockMinimap = false;
			}

			//------------------------------------------------

			if (!blockMinimap) {
				camera.go_to_pos(camera_to_x, camera_to_y);
				GAME::MINIMAP_IS_ACTIVE = false;
			}

			GLB::MOUSE_LEFT = false;
		}
	}
}

void Game::render_selection_rectangle() {
	if (!GAME::MINIMAP_IS_ACTIVE) {
		if (GLB::MOUSE_LEFT && cursorInGameScreen()) {
			sel_rect_coords[0] = GLB::MOUSE_LEFT_X + cameraLastX;
			sel_rect_coords[1] = GLB::MOUSE_LEFT_Y + cameraLastY;
			sel_rect_coords[2] = GLB::MOUSE_LEFT_X + cameraLastX;
			sel_rect_coords[3] = GLB::MOUSE_Y + GAME::CAMERA_POS_Y;
			sel_rect_coords[4] = GLB::MOUSE_X + GAME::CAMERA_POS_X;
			sel_rect_coords[5] = GLB::MOUSE_Y + GAME::CAMERA_POS_Y;
			sel_rect_coords[6] = GLB::MOUSE_X + GAME::CAMERA_POS_X;
			sel_rect_coords[7] = GLB::MOUSE_LEFT_Y + cameraLastY;

			if (GLB::MOUSE_Y < GAME::UI_BOTTOM_HEIGHT) {
				sel_rect_coords[3] = GAME::UI_BOTTOM_HEIGHT + 1.0f + GAME::CAMERA_POS_Y;
				sel_rect_coords[5] = GAME::UI_BOTTOM_HEIGHT + 1.0f + GAME::CAMERA_POS_Y;
			}
			if (GLB::MOUSE_Y > GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT) {
				sel_rect_coords[3] = GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 1.0f + GAME::CAMERA_POS_Y;
				sel_rect_coords[5] = GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 1.0f + GAME::CAMERA_POS_Y;
			}
			selectionRectangle.create(sel_rect_coords);
			selectionRectangle.render(view, glm::mat4(1.0f));
		}
		else {
			cameraLastX = GAME::CAMERA_POS_X;
			cameraLastY = GAME::CAMERA_POS_Y;
		}
	}
}

void Game::render_minimap_rectangle() {
	if (GAME::MINIMAP_IS_ACTIVE){
		/* minimap rectangle coordinates */
		minimap_rect_coords[0] = 0.0f;	minimap_rect_coords[1] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT;
		minimap_rect_coords[2] = 0.0f; minimap_rect_coords[3] = GAME::UI_BOTTOM_HEIGHT;
		minimap_rect_coords[4] = GLB::WINDOW_WIDTH_ZOOMED; minimap_rect_coords[5] = GAME::UI_BOTTOM_HEIGHT;
		minimap_rect_coords[6] = GLB::WINDOW_WIDTH_ZOOMED; minimap_rect_coords[7] = GLB::WINDOW_HEIGHT_ZOOMED - GAME::UI_TOP_HEIGHT;
		minimapRectangle_model = glm::translate(glm::mat4(1.0f), glm::vec3(GAME::CAMERA_POS_X, GAME::CAMERA_POS_Y, 0.0));
		minimapRectangle.create(minimap_rect_coords);
		minimapRectangle.render(glm::mat4(1.0f), minimapRectangle_model);
	}
}

void Game::clear_buffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

Game::~Game()
{
}