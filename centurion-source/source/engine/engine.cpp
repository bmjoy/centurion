#include "engine.h"


Engine::Engine(){
	mouse = Mouse();
	window = myWindow();
	
	image_setup = Image();
	unit_sprite_setup = USprite();

	nbFrames = 0; 
	Fps = 0;
	Mpfs = 0;
}


int Engine::launch() {
	window.init();

	GLB::MAIN_WINDOW = window.get_GLFW_window();

	std::cout << "Window has been initialised. \n";

	obj::init();

	readDataClasses();
	compile_shaders();
	init_objects();

	lastTime = glfwGetTime();

	seconds = 0; seconds_str = "00";
	minutes = 0; minutes_str = "00";
	hours = 0; hours_str = "00";

	std::cout << "I am in the while() loop! \n";

	startMenu = new Menu();
	game = new Game();

	debugUI = DebugUI();
	debugUI.create();

	while (!GLB::WINDOW_CLOSE) {

		glfwPollEvents();
		window.clear_buffers();
		fps();
		mouse.mouse_control(window.get_mouse_x(), window.get_mouse_y());
		mouse.apply_projection_matrix(GLB::MENU_PROJECTION);
		
		// ---- MENU ---- //
		
		if (GLB::MAIN_MENU){
			if (!startMenu->menu_is_created()){

				obj::ERectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
				obj::ERectangle()->apply_view_matrix();

				obj::FRectangle()->apply_projection_matrix(GLB::MENU_PROJECTION);
				obj::FRectangle()->apply_view_matrix();

				
				startMenu->create(&playersList);
			}
			startMenu->render();
		}

		// ---- GAME ---- //

		if (GLB::GAME) {
			
			if (!game->game_is_created()) {

				clearAndSwapBuffers(GLB::MAIN_WINDOW);
				{
					text.set_position(glm::vec2(GLB::WINDOW_WIDTH / 2.f, GLB::WINDOW_HEIGHT / 2.f));
					text.set_text("Game is being created...");
					text.render_dynamic("tahoma_8", glm::vec4(255.f), "center", "middle");
				}
				glfwSwapBuffers(GLB::MAIN_WINDOW);

				game->create(&playersList);
	
				lastTime2 = glfwGetTime();

			}			
			
			game->run();	
			calculateTime();

		}

		// -------------- //

		if (GLB::GAME_CLEAR) {
			GLB::GAME_CLEAR = false;
			GLB::GAME = false;
			GLB::MAIN_MENU = true;

			game->reset();
			startMenu->reset();
		}

		// -------------- //

		// debug ui
		debugUI.render(Fps, Mpfs, game->getSelectedUnits());

		// mouse
		mouse.render();
		
		glfwSwapBuffers(GLB::MAIN_WINDOW);
		//fps_sleep();
	}

	glfwTerminate(); 
	return 0;
}

void Engine::compile_shaders() {
	SHD::IMAGE_SHADER_ID = image_setup.compile();
	SHD::USPRITE_SHADER_ID = unit_sprite_setup.compile();
}

void Engine::readDataClasses() {

	std::vector<std::string> files = get_all_files_names_within_folder("assets/data/classes");
	for (int i = 0; i < files.size(); ++i) {
		
		std::ifstream path("assets/data/classes/" + files[i]);
		json dataClass = json::parse(path);

		if (dataClass["type"] == "building") {			
			obj::BuildingSprite()->addPath(dataClass["ent_path"]);
		}
	}	
}

void Engine::init_objects() {

	obj::compile();
	obj::create();

	mouse.compile();
	mouse.create();
	std::cout << "Mouse is ready!\n";

	text = gui::SimpleText("dynamic");
}


void Engine::fps() {
	currentTime = glfwGetTime();
	nbFrames++;

	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		Fps = nbFrames;
		Mpfs = 1000 / nbFrames;

		lastTime += 1.0;
		nbFrames = 0;
	}
}

void Engine::calculateTime() {
	if (glfwGetTime() - lastTime2 >= 1.0f) {
		seconds++;
		lastTime2 += 1.0f;
	}
	if (seconds == 60) {
		seconds = 0;
		minutes++;
	}
	if (minutes == 60) {
		minutes = 0;
		hours++;
	}
	if (seconds < 10) { seconds_str = "0" + std::to_string(seconds); }
	else { seconds_str = std::to_string(seconds); }
	if (minutes < 10) { minutes_str = "0" + std::to_string(minutes); }
	else { minutes_str = std::to_string(minutes); }
	if (hours < 10) { hours_str = "0" + std::to_string(hours); }
	else { hours_str = std::to_string(hours); }
}

void Engine::fps_sleep() {
	finalTime = glfwGetTime();
	if (finalTime - currentTime < 1.0 / 60.0) {
		Sleep(1000 * (1.0 / 60.0 - (finalTime - currentTime)));
	}
}

void Engine::render_ui() {

	/* DEBUG */

	/* TIME */
	if (GLB::GAME){
		
		timeText.set_position(glm::vec2(GLB::WINDOW_WIDTH - 99.0f, GLB::WINDOW_HEIGHT - GAME::UI_TOP_HEIGHT - 29.0f));
		timeText.set_text(hours_str + ":" + minutes_str + ":" + seconds_str);
		timeText.render_dynamic("tahoma_8", glm::vec4(0.f,0.f,0.f,255.f), "left", "normal");
	}

}

Engine::~Engine()
{
	delete startMenu;
	delete game;
}