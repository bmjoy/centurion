#include <engine>
#include <editor>
#include <game>
#include <player>
#include "../menu/menu.h"
#include "../interface/debug_ui.h"

using namespace glb;
using namespace game;
using namespace editor;

namespace engine {
	Engine *ENGINE() { return &myengine; }

	Engine::Engine(){
		window = myWindow();
		nbFrames = 0; 
		Fps = 0;
		Mpfs = 0;
		environment = "menu";
		reset = false;
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
		init();
		editor = new Editor();
	
		debugUI = new DebugUI();
		debugUI->create();

		while (!getBoolean("window-should-close")) {

			glfwPollEvents();
			window.clear_buffers();
			fps();
			mouse->mouse_control(window.get_mouse_x(), window.get_mouse_y());
		
			// ---- MENU ---- //
		
			if (environment == "menu"){
				if (!startMenu->menu_is_created()){
					obj::Audio()->MusicPlay("assets/music/menu.wav");
					startMenu->create(&playersList);
					std::cout << "DEBUG: Main menu was created!\n";
				}
				startMenu->render();
			}

			// ---- GAME ---- //

			if (environment == "game") {
				if (!GAME()->game_is_created()) {
					obj::Audio()->MusicStop();

					clearAndSwapBuffers(MainWindow);
					{
						text = gui::SimpleText("dynamic");
						text.render_dynamic("Game is being started...", "tahoma_8", getParam("window-width") / 2.f, getParam("window-height") / 2.f, glm::vec4(255.f), "center", "middle");
					}
					glfwSwapBuffers(MainWindow);

					GAME()->create(&playersList);
				}						
				GAME()->run();
			}

			// -------------- //

			if (environment == "editor") {
				if (!editor->editor_is_created()) {
					obj::Audio()->MusicStop();
					editor->create();
				}
				editor->run();
			}

			// -------------- //

			if (reset) {
				reset = false;				
				if (environment == "editor") editor->reset();
				if (environment == "game") GAME()->reset();
				startMenu->reset();				
				environment = "menu";
			}

			// debug ui
			if (getBoolean("debug"))	debugUI->render(Fps, Mpfs, GAME()->getSelectedUnits());

			// mouse
			mouse->render();
		
			glfwSwapBuffers(MainWindow);

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
		if (!path2.good()) {
			forceGameClosure("Error code 0x00000001\n\n  Unable to find or process IMAGES DATA file.\n  Forced application shutdown has started.");
		}
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
			Sleep(DWORD(1000 * (1.0 / 60.0 - (finalTime - currentTime))));
		}
	}

	Engine::~Engine()
	{
		delete startMenu;		
		delete mouse;
		delete debugUI;
	}
}