#include "engine.h"


Engine::Engine(){
	window = myWindow();
	
	nbFrames = 0; 
	Fps = 0;
	Mpfs = 0;
}


int Engine::launch() {
	window.init();
	obj::init();
	
	readDataClasses();
	
	obj::compile();
	obj::create();

	lastTime = glfwGetTime();

	mouse = new Mouse();
	startMenu = new Menu();
	game = new Game();
	editor = new Editor();
	
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
				music = Music().play("assets/music/menu.mp3", true);
				startMenu->create(&playersList);
			}
			startMenu->render();
		}

		// ---- GAME ---- //

		if (GLB::GAME) {
			if (!game->game_is_created()) {
				Music().stop(music);
				music = Music().play("assets/music/game.mp3", true);

				clearAndSwapBuffers(GLB::MAIN_WINDOW);
				{
					text = gui::SimpleText("dynamic");
					text.render_dynamic("Game is being created...", "tahoma_8", GLB::WINDOW_WIDTH / 2.f, GLB::WINDOW_HEIGHT / 2.f, glm::vec4(255.f), "center", "middle");
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
			Music().stop(music);
		}

		// -------------- //

		if (GLB::EDITOR) {
			if (!editor->editor_is_created()) {
				editor->create();
			}
			editor->run();
		}

		// debug ui
		if (GLB::DEBUG)	debugUI->render(Fps, Mpfs, game->getSelectedUnits());

		// mouse
		mouse->render();
		
		glfwSwapBuffers(GLB::MAIN_WINDOW);

		fps_sleep();
	}

	glfwTerminate(); 
	return 0;
}

void Engine::readDataClasses() {

	std::vector<std::string> files = get_all_files_names_within_folder("assets/data/classes");
	json dataClass;
	
	/* buildings and units */

	for (int i = 0; i < files.size(); ++i) {
		
		std::ifstream path("assets/data/classes/" + files[i]);
		dataClass = json::parse(path);

		if (dataClass["type"] == "building") {			
			obj::BSprite()->addPath(dataClass["ent_path"]);
		}

		if (dataClass["type"] == "unit") {
			obj::USprite()->addPath(dataClass["ent_path"]);
		}
	}

	/* images */

	std::ifstream path2("assets/data/images_data.json");
	dataClass = json::parse(path2);

	for (int i = 0; i < dataClass["image_names"].size(); i++) {
		std::string imageName = dataClass["image_names"][i].get<std::string>();
		std::string root = dataClass["roots"][imageName];

		obj::Img()->addPath(imageName, root);
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