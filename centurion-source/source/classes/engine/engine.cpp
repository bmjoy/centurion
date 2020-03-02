#include "engine.h"
#include "window.h"
#include "camera.h"
#include "mouse.h"

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
	// define static variables
	gui::SimpleText Engine::text;
	string Engine::environment;
	double Engine::currentTime, Engine::lastTime, Engine::finalTime;
	int Engine::nbFrames, Engine::Fps, Engine::Mpfs;
	bool Engine::reset;
	// ---------- end definitions

	Engine::Engine() { }

	void Engine::Init() {
		nbFrames = 0;
		Fps = 0;
		Mpfs = 0;
		environment = "menu";
		reset = false;
	}

	int Engine::launch() {
		myWindow window = myWindow::GetInstance();
		obj::init();

		read_data();

		obj::compile();
		obj::create();

		lastTime = glfwGetTime();

		Camera::Init(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
		Mouse::create();

		DEBUG_UI()->create();

		while (myWindow::ShouldClose == false) {
			glfwPollEvents();
			window.clear_buffers();
			fps();
			Mouse::mouse_control(window.get_mouse_x(), window.get_mouse_y());
			handleGlobalKeys();

			// ---- MENU ---- //

			if (environment == "menu") {
				if (!MENU()->menu_is_created()) {
					obj::Audio()->MusicPlay("assets/music/menu.ogg");
					MENU()->create();
					cout << "[DEBUG] Main menu was created!\n";
				}
				MENU()->render();
			}

			// ---- GAME ---- //

			if (environment == "game") {
				if (!GAME()->game_is_created()) {
					obj::Audio()->MusicStop();

					clearAndSwapBuffers(window.GetGlfwWindow());
					{
						text = gui::SimpleText("dynamic");
						text.render_dynamic(getTranslation("WORD_gameStarted"), "tahoma_15px", myWindow::Width / 2.f, myWindow::Height / 2.f, vec4(255.f), "center", "middle");
					}
					glfwSwapBuffers(window.GetGlfwWindow());

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
				if (environment == "game") { GAME()->reset(); GAME()->clear(); }
				MENU()->reset();
				environment = "menu";
			}

			// debug ui
			if (Settings::DebugIsActive)	DEBUG_UI()->render(Fps, Mpfs, selectedUnits);

			// mouse
			Mouse::render();

			if ((KeyCode[GLFW_KEY_LEFT_SHIFT] || KeyCode[GLFW_KEY_RIGHT_SHIFT]) && KeyCode[GLFW_KEY_S]) {
				cout << "[DEBUG] Screenshot taken." << endl;
				glb::takeScreenshot();
			}

			CharCodepointPressed = -1;
			resetKeyCodes();
			glfwSwapBuffers(window.GetGlfwWindow());

			fps_sleep();
		}

		if (MENU()->menu_is_created()) MENU()->reset();

		glfwTerminate();
		return 0;
	}

	void Engine::read_data() {
		//Read races data
		vector<string> r_files = get_all_files_names_within_folder("assets/data/races");
		for (int i = 0; i < r_files.size(); ++i) {
			ifstream path("assets/data/races/" + r_files[i]);
			json dataRaces = json::parse(path);

			Race r = Race();
			int id = dataRaces["race_id"].get<int>();
			string name = dataRaces["race_name"].get<string>();
			string zone = dataRaces["zone"].get<string>();
			string t_class = dataRaces["food_transport_class"].get<string>();
			r.setRaceProperties(id, name, zone, t_class);
			RACES[name] = r;
			racesNames.push_back("RACE_" + name);
		}

		vector<string> files = get_all_files_names_within_folder("assets/data/classes");

		/* buildings and units */

		for (int i = 0; i < files.size(); ++i) {
			ifstream path("assets/data/classes/" + files[i]);
			json dataClass = json::parse(path);

			if (dataClass["type"] == "building") {
				obj::BSprite()->addPath(dataClass["ent_path"]);

				/* editor object string list */
				EditorObjectStringListForm0[i] = "buildings";
				EditorObjectStringListForm1[i] = dataClass["category"].get<string>();
				EditorObjectStringListForm2[i] = dataClass["class_name"].get<string>();

				EditorAddObjectBuildingOptions.push_back("EDITORTREE_CATEGORY_" + dataClass["category"].get<string>());

				if (dataClass["category"].get<string>() == "outposts") outposts.push_back(dataClass["class_name"].get<string>());
			}

			if (dataClass["type"] == "unit") {
				obj::USprite()->addPath(dataClass["ent_path"]);

				//EditorAddObjectUnitOptions.push_back(dataClass["race"].get<string>());
			}

			if (dataClass["type"] == "decoration") {
				obj::DSprite()->addPath(dataClass["ent_path"]);
				/* editor object string list */
				EditorObjectStringListForm0[i] = "decorations";
				EditorObjectStringListForm1[i] = dataClass["category"].get<string>();
				EditorObjectStringListForm2[i] = dataClass["class_name"].get<string>();

				EditorAddObjectDecorationOptions.push_back("EDITORTREE_CATEGORY_" + dataClass["category"].get<string>());
			}
		}

		EditorAddObjectBuildingOptions.erase(unique(EditorAddObjectBuildingOptions.begin(), EditorAddObjectBuildingOptions.end()), EditorAddObjectBuildingOptions.end());
		//EditorAddObjectUnitOptions.erase(unique(EditorAddObjectUnitOptions.begin(), EditorAddObjectUnitOptions.end()), EditorAddObjectUnitOptions.end());
		EditorAddObjectDecorationOptions.erase(unique(EditorAddObjectDecorationOptions.begin(), EditorAddObjectDecorationOptions.end()), EditorAddObjectDecorationOptions.end());

		/* images */

		vector<file_info> imagesInfoList = get_all_files_names_within_subfolders("assets\\ui", "png");

		for (int i = 0; i < imagesInfoList.size(); i++)
			obj::Img()->addPath(imagesInfoList[i].name, imagesInfoList[i].path);

		/* terrain textures */

		vector<file_info> texturesInfoList = get_all_files_names_within_subfolders("assets\\terrain\\textures", "png");

		for (int i = 0; i < texturesInfoList.size(); i++)
			obj::MapTerrain()->addPath(texturesInfoList[i].name, texturesInfoList[i].path);
	}

	void Engine::resetKeyCodes() {
		KeyCode[GLFW_KEY_SPACE] = false;
		KeyCode[GLFW_KEY_Z] = false;
		KeyCode[GLFW_KEY_A] = false;
		KeyCode[GLFW_KEY_S] = false;
		KeyCode[GLFW_KEY_T] = false;
		KeyCode[GLFW_KEY_F10] = false;
		KeyCode[GLFW_KEY_ENTER] = false;
		KeyCode[GLFW_KEY_ESCAPE] = false;
	}

	void Engine::handleGlobalKeys() {
		// activate or deactivate debug ui
		if (Settings::DebugIsActive && KeyCode[GLFW_KEY_F10]) {
			debug::DEBUG_UI()->setStatus(!debug::DEBUG_UI()->getStatus());
			debug::DEBUG_UI()->getStatus() ? std::cout << "[DEBUG] Debug UI ON!\n" : std::cout << "[DEBUG] Debug UI OFF!\n";
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

	Engine::~Engine() { }
}
