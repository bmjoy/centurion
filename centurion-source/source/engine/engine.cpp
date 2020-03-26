#include "engine.h"
#include <file_manager.h>

#include <translationsTable.h>
#include <classes/object-data.h>

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif
#include <Windows.h>

#include <GLFW/glfw3.h>

#include <game/game.h>
#include <game/editor.h>
#include <game/strategy.h>
#include <player/player.h>

#include <menu/menu.h>
#include <game/interface/editorWindows.h>

#include <interface>
#include <classes/unit.h>
#include <settings.h>
#include <errorCodes.h>
#include <logger.h>
#include <hector-lua.h>


// audio
#include "audio_manager.h"

// read data (temporary: waiting for xml reading system)
#include "building_sprite.h"
#include "unit_sprite.h"
#include "decoration_sprite.h"
#include "terrain.h"
#include "image_sprite.h"
// 

using namespace menu;
using namespace debug;


int Engine::ENVIRONMENT = MENU_ENV;
bool Engine::ENGINE_RESET;

int Engine::Launch() {
	myWindow window = myWindow::GetInstance();
	init();

	ObjectData::ReadDataClassesFromXml();
	read_data();

	primitives::compile();
	primitives::create();

	Camera::Init(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	Mouse::create();

	DEBUG_UI()->create();

	std::ostringstream ss;
	ss << glGetString(GL_VERSION);
	Logger::Info("Running OpenGL Version " + ss.str());

	// Hector - Lua interpreter
	Hector::Initialize();
	Hector::CreateConsole();

	Fps _fps = Fps();
	_fps.SetLastTime(glfwGetTime());

	while (myWindow::ShouldClose == false) {
		glfwPollEvents();
		Mouse::IsHolding(); 
		window.ClearBuffers();
		_fps.Update();
		Mouse::mouse_control(window.get_mouse_x(), window.get_mouse_y());
		handleGlobalKeys();

		// ---- MENU ---- //

		if (getEnvironment() == MENU_ENV) {
			
			if (Menu::IsCreated() == false)
			{
				//Audio()->MusicPlay("assets/music/menu.ogg");
				Menu::Create();
				Logger::Info("Main menu was created!");
			}
			Menu::Run();
		}

		// ---- STRATEGY ---- //

		if (getEnvironment() == STRATEGY_ENV) {
			if (!Strategy::IsCreated()) {
				//Audio()->MusicStop();

				//window.ClearAndSwapBuffers();

				//{
					//text = gui::SimpleText("dynamic");
					//text.render_dynamic(TranslationsTable::GetTranslation("WORD_gameStarted"), "tahoma_15px", myWindow::Width / 2.f, myWindow::Height / 2.f, vec4(255.f), "center", "middle");
				//}

				//window.SwapBuffers();

				Strategy::Create();
			}
			Strategy::Run();
		}

		// ---- EDITOR ---- //

		if (getEnvironment() == EDITOR_ENV) {
			if (!Editor::IsCreated()) {
				Audio()->MusicStop();
				Editor::Create();
			}
			Editor::Run();
		}

		// -------------- //

		if (ENGINE_RESET) {
			ENGINE_RESET = false;
			if (getEnvironment() == EDITOR_ENV) Editor::reset(); editor::clearEditorVariables();
			if (getEnvironment() == STRATEGY_ENV) Strategy::Reset();
			//MENU()->reset();

			SetEnvironment("menu");
		}

		// debug ui
		if (Settings::DebugIsActive) DEBUG_UI()->render(_fps.GetFps(), _fps.GetMpfs(), Unit::GetCounter());

		Hector::RenderConsole();

		// mouse
		Mouse::render();

		if (Hector::ConsoleIsActive() == false) {
			if ((Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT_SHIFT) || Keyboard::IsKeyNotReleased(GLFW_KEY_RIGHT_SHIFT)) && Keyboard::IsKeyNotReleased(GLFW_KEY_S)) {
				Logger::Info("Screenshot taken!");
				myWindow::TakeScreenshot();
			}
		}

		Keyboard::SetCharCodepointPressed(-1);
		ResetPeriphericsInput();

		window.SwapBuffers();

		_fps.SleepFps();
	}

	//if (MENU()->menu_is_created()) MENU()->reset();
	Logger::SaveParamsXML();
	
	GObject::ResetGameObjects();
	Menu::Clear();
	myWindow::DeleteInstance();
	EditorWindows::Clear();

	glfwTerminate();
	return 0;
}


void Engine::GameClose()
{
	myWindow::ShouldClose = true;
}

void Engine::SetEnvironment(string s)
{
	if (s == "menu") {
		ENVIRONMENT = MENU_ENV;
	}
	else if (s == "editor") {
		ENVIRONMENT = EDITOR_ENV;
	}
	else if (s == "strategy") {
		ENVIRONMENT = STRATEGY_ENV;
	}
	else {
		ENVIRONMENT = MENU_ENV;
	}
}

void Engine::PrintToConsole(string s)
{
	cout << s << endl;
}

vector<string>* Engine::GetListOfFolders(string s)
{
	if (FileManager::CheckIfFolderExists(s)) {
		vector<string> list = FileManager::GetAllFoldersNamesWithinFolder(s);
		vector<string>* _list = new vector<string>();
		(*_list) = list;
		return _list;
	}
	return nullptr;
}

void Engine::Init(const char* exe_root) {

	// old initParams (global)

	try {

		Settings::SetFolders(FileManager::GetFileFolderPath(exe_root));
		ErrorCodes::ReadErrorCodesXml();
		Logger::CleanLogs();

		Settings::Init();
		Settings::ReadSettings();

		Engine::myWindow::Ratio = Engine::myWindow::Width / Engine::myWindow::Height;
		Engine::myWindow::WidthZoomed = Engine::myWindow::Width + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor();
		Engine::myWindow::HeightZoomed = Engine::myWindow::Height + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor() / Engine::myWindow::Ratio;

		setMenuProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::Width, 0.0f, Engine::myWindow::Height, -100.0f, 100.0f));
		setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));


		ifstream data_path("assets/data/data.json");
		//Close the game if it wasn't able to find or process data.json file
		if (!data_path.good()) {
			//forceGameClosure("NOT_FOUND", "ERROR_data");
		}
		json data = json::parse(data_path);

		for (int i = 0; i < data["player_colors"].size(); i++) {
			vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
			Game::AddColor(color);
		}


	}
	catch (...) {
		std::cout << "An error occurred" << std::endl;
	}

	//------------------------------

	SetEnvironment("menu");
}

void Engine::Reset()
{
	ENGINE_RESET = true;
}

int Engine::getEnvironment()
{
	return ENVIRONMENT;
}

void Engine::read_data() {
	//Read races data
	vector<string> r_files = FileManager::GetAllFilesNamesWithinFolder("assets/data/races");
	for (int i = 0; i < r_files.size(); ++i) {
		ifstream path("assets/data/races/" + r_files[i]);
		json dataRaces = json::parse(path);

		Game::Race r = Game::Race();
		int id = dataRaces["race_id"].get<int>();
		string name = dataRaces["race_name"].get<string>();
		string zone = dataRaces["zone"].get<string>();
		string t_class = dataRaces["food_transport_class"].get<string>();
		r.setRaceProperties(id, name, zone, t_class);
		Game::AddRace(name, r);
	}

	/* images */

	vector<FileManager::file_info> imagesInfoList = FileManager::GetAllFilesNamesWithinSubfolders("assets\\ui", "png");

	for (int i = 0; i < imagesInfoList.size(); i++)
		Img()->addPath(imagesInfoList[i].name, imagesInfoList[i].path);

	/* terrain textures */

	vector<FileManager::file_info> texturesInfoList = FileManager::GetAllFilesNamesWithinSubfolders("assets\\terrain\\textures", "png");

	for (int i = 0; i < texturesInfoList.size(); i++)
		MapTerrain()->addPath(texturesInfoList[i].name, texturesInfoList[i].path);
}

void Engine::handleGlobalKeys() {
	// activate or deactivate debug ui
	if (Settings::DebugIsActive && Keyboard::IsKeyPressed(GLFW_KEY_F10)) {
		debug::DEBUG_UI()->setStatus(!debug::DEBUG_UI()->getStatus());
		debug::DEBUG_UI()->getStatus() ? Logger::Info("Debug UI ON!") : Logger::Info("Debug UI OFF!");
	}
}

void Engine::ResetPeriphericsInput()
{
	//Keyboard buttons
	Keyboard::ResetKeys();

	//Mouse buttons
	Mouse::LeftClick = false;
	Mouse::RightClick = false;
	Mouse::MiddleClick = false;
	Mouse::Release = false;
}

Engine::Fps::Fps()
{
	this->nbFrames = 0;
	this->_Fps = 0;
	this->Mpfs = 0;
}

void Engine::Fps::Update(void) {
	this->currentTime = glfwGetTime();
	this->nbFrames++;
	if (this->currentTime - this->lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		this->_Fps = this->nbFrames;
		this->Mpfs = 1000 / this->nbFrames;
		this->lastTime += 1.0;
		this->nbFrames = 0;
	}
}

void Engine::Fps::SleepFps(void) {
	this->finalTime = glfwGetTime();
	if (this->finalTime - this->currentTime < 1.0 / 60.0) {
		Sleep(DWORD(1000 * (1.0 / 60.0 - (this->finalTime - this->currentTime))));
	}
}

Engine::Fps::~Fps() {}

