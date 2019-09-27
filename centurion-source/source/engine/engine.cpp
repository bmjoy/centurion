#include "engine.h"


Engine::Engine(){
	window = myWindow();
	
	image_setup = Image();

	nbFrames = 0; 
	Fps = 0;
	Mpfs = 0;
}


int Engine::launch() {
	window.init();
	obj::init();
	
	readDataClasses();
	
	compile_shaders(); // temporary
	
	obj::compile();
	obj::create();

	


	lastTime = glfwGetTime();

	startMenu = new Menu();
	game = new Game();
	mouse = new Mouse();
	debugUI = new DebugUI();
	debugUI->create();

	GLB::MAIN_WINDOW = window.get_GLFW_window();

	while (!GLB::WINDOW_CLOSE) {

		glfwPollEvents();
		window.clear_buffers();
		fps();
		mouse->mouse_control(window.get_mouse_x(), window.get_mouse_y());
		
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
			}						
			game->run();	
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
		if (GLB::DEBUG)	debugUI->render(Fps, Mpfs, game->getSelectedUnits());

		// mouse
		mouse->render();
		
		glfwSwapBuffers(GLB::MAIN_WINDOW);

		//fps_sleep();
	}

	glfwTerminate(); 
	return 0;
}

void Engine::compile_shaders() {
	SHD::IMAGE_SHADER_ID = image_setup.compile();
}

void Engine::readDataClasses() {

	std::vector<std::string> files = get_all_files_names_within_folder("assets/data/classes");
	for (int i = 0; i < files.size(); ++i) {
		
		std::ifstream path("assets/data/classes/" + files[i]);
		json dataClass = json::parse(path);

		if (dataClass["type"] == "building") {			
			obj::BSprite()->addPath(dataClass["ent_path"]);
		}

		if (dataClass["type"] == "unit") {
			obj::USprite()->addPath(dataClass["ent_path"]);
		}
	}	
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

void Engine::fps_sleep() {
	finalTime = glfwGetTime();
	if (finalTime - currentTime < 1.0 / 60.0) {
		Sleep(1000 * (1.0 / 60.0 - (finalTime - currentTime)));
	}
}

Engine::~Engine()
{
	delete startMenu;
	delete game;
	delete mouse;
	delete debugUI;
}