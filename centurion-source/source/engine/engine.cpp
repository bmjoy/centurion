#include <engine>
#include <editor>
#include <game>
#include <player>
#include <menu>
#include <interface>

using namespace std;
using namespace glm;
using namespace glb;
using namespace menu;
using namespace game;
using namespace editor;
using namespace debug;

namespace engine {
	Engine *ENGINE() { return &myengine; }
	Camera *CAMERA() { return &mycamera; }

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

		*CAMERA() = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
		mouse = new Mouse();

		DEBUG_UI()->create();

		while (!getBoolean("window-should-close")) {

			glfwPollEvents();
			window.clear_buffers();
			fps();
			mouse->mouse_control(window.get_mouse_x(), window.get_mouse_y());

			// ---- MENU ---- //
		
			if (environment == "menu"){
				if (!MENU()->menu_is_created()){
					obj::Audio()->MusicPlay("assets/music/menu.ogg");
					MENU()->create();
					cout << "DEBUG: Main menu was created!\n";
				}
				MENU()->render();
			}

			// ---- GAME ---- //

			if (environment == "game") {
				if (!GAME()->game_is_created()) {
					obj::Audio()->MusicStop();

					clearAndSwapBuffers(MainWindow);
					{
						text = gui::SimpleText("dynamic");
						text.render_dynamic(getTranslation("WORD_gameStarted"), "tahoma_15px", getParam("window-width") / 2.f, getParam("window-height") / 2.f, vec4(255.f), "center", "middle");
					}
					glfwSwapBuffers(MainWindow);

					GAME()->create();
				}						
				GAME()->run();
			}

			// ---- EDITOR ---- //

			if (environment == "editor") {
				if (!EDITOR()->editor_is_created()) {
					obj::Audio()->MusicStop();
					EDITOR()->create();
				}
				EDITOR()->run();
			}

			// -------------- //

			if (reset) {
				reset = false;				
				if (environment == "editor") EDITOR()->reset();
				if (environment == "game") GAME()->reset();
				MENU()->reset();
				environment = "menu";
			}

			// debug ui
			if (getBoolean("debug"))	DEBUG_UI()->render(Fps, Mpfs, selectedUnits);

			// mouse
			mouse->render();
		
			glfwSwapBuffers(MainWindow);

			fps_sleep();
		}

		glfwTerminate(); 
		return 0;
	}

	void Engine::readDataClasses() {

		vector<string> files = get_all_files_names_within_folder("assets/data/classes");
		json dataClass;
	
		/* buildings and units */

		for (int i = 0; i < files.size(); ++i) {
		
			ifstream path("assets/data/classes/" + files[i]);
			dataClass = json::parse(path);

			if (dataClass["type"] == "building") {			
				obj::BSprite()->addPath(dataClass["ent_path"]);
			}

			if (dataClass["type"] == "unit") {
				obj::USprite()->addPath(dataClass["ent_path"]);
			}
		}

		/* images */

		ifstream path2("assets/data/images_data.json");
		if (path2.good()) {
			forceGameClosure("NOT_FOUND", "ERROR_imagesData");
		}
		dataClass = json::parse(path2);

		for (int i = 0; i < dataClass["image_names"].size(); i++) {
			string imageName = dataClass["image_names"][i].get<string>();
			string root = dataClass["roots"][imageName];

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

	Engine::~Engine(){		
		delete mouse;
	}
}
