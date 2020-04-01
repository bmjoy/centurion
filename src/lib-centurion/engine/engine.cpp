#include "engine.h"

#include "debugUi.h"

#include <file_manager.h>

#include <translationsTable.h>
#include <classes/object-data.h>

#include <GLFW/glfw3.h>

#include <tinyxml2.h>

#include <game/game.h>
#include <game/editor.h>
#include <game/strategy.h>
#include <player/player.h>

#include <menu/menu.h>
#include <game/interface/editorWindows.h>

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

#include <chrono>
#include <thread>

using namespace std;
using namespace glm;

int Engine::ENVIRONMENT = MENU_ENV;

int Engine::Launch(void) 
{
	myWindow window = myWindow::GetInstance();
	init();

	ObjectData::ReadDataClassesFromXml();
	read_data();

	primitives::compile();
	primitives::create();

	Camera::Init(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	Mouse::create();

	DebugUI debugUI = DebugUI();
	debugUI.create();

	std::ostringstream ss;
	ss << glGetString(GL_VERSION);
	Logger::Info("Running OpenGL Version " + ss.str());

	Logger::Info("C++ Compiler Version: " + GetCppVersion());

	// Hector - Lua interpreter
	Hector::Initialize();
	Hector::CreateConsole();

	Fps _fps = Fps();
	_fps.SetLastTime(glfwGetTime());

	while (myWindow::ShouldClose == false) 
	{
		glfwPollEvents();
		Mouse::IsHolding(); 
		window.ClearBuffers();
		_fps.Update();
		Mouse::mouse_control(window.get_mouse_x(), window.get_mouse_y());

		// ---- MENU ---- //

		if (GetEnvironment() == MENU_ENV)
		{
			Menu::Run();
		}

		// ---- STRATEGY ---- //

		if (GetEnvironment() == STRATEGY_ENV)
		{
			//Strategy::Run();
		}

		// ---- EDITOR ---- //

		if (GetEnvironment() == EDITOR_ENV) 
		{
			Editor::Run();
		}

		// -------------- //

		// debug ui
		if (Settings::DebugIsActive)
		{
			debugUI.render(_fps.GetFps(), _fps.GetMpfs(), Unit::GetCounter());
		}

		Hector::RenderConsole();

		// mouse
		Mouse::render();

		if (Hector::ConsoleIsActive() == false) 
		{
			if ((Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT_SHIFT) || Keyboard::IsKeyNotReleased(GLFW_KEY_RIGHT_SHIFT)) && Keyboard::IsKeyNotReleased(GLFW_KEY_S)) 
			{
				Logger::Info("Screenshot taken!");
				myWindow::TakeScreenshot();
			}
		}

		Keyboard::SetCharCodepointPressed(-1);
		ResetperipheralsInput();

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


void Engine::GameClose(void)
{
	myWindow::ShouldClose = true;
}

void Engine::SetEnvironment(const string s)
{
	if (s == "menu") 
	{
		ENVIRONMENT = MENU_ENV;
	}
	else if (s == "editor") 
	{
		ENVIRONMENT = EDITOR_ENV;
	}
	else if (s == "strategy") 
	{
		ENVIRONMENT = STRATEGY_ENV;
	}
	else 
	{
		ENVIRONMENT = MENU_ENV;
	}
}

void Engine::PrintToConsole(const string s)
{
	cout << s << endl;
}

vector<string>* Engine::GetListOfFolders(const string s)
{
	if (FileManager::CheckIfFolderExists(s))
	{
		vector<string> list = FileManager::GetAllFoldersNamesWithinFolder(s);
		vector<string>* _list = new vector<string>();
		(*_list) = list;
		return _list;
	}
	return nullptr;
}

void Engine::Init(const char* exe_root)
{
	// old initParams (global)
	try
	{
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

		// read data.xml

		string path = Folders::DATA + "data.xml";
		tinyxml2::XMLDocument xmlFile;
		xmlFile.LoadFile(path.c_str());

		// colors 

		tinyxml2::XMLElement *_colorsArray = xmlFile.FirstChildElement("data")->FirstChildElement("playerColorArray");
		for (tinyxml2::XMLElement* _color = _colorsArray->FirstChildElement(); _color != NULL; _color = _color->NextSiblingElement())
		{
			int r = stoi(_color->Attribute("r"));
			int g = stoi(_color->Attribute("g"));
			int b = stoi(_color->Attribute("b"));
			vec3 color = vec3(r, g, b);
			Game::AddColor(color);
		}

		// races

		tinyxml2::XMLElement *_racesArray = xmlFile.FirstChildElement("data")->FirstChildElement("raceArray");
		for (tinyxml2::XMLElement* _race = _racesArray->FirstChildElement(); _race != NULL; _race = _race->NextSiblingElement())
		{
			Game::Race r = Game::Race();
			int id = stoi(_race->Attribute("id"));
			string name = string(_race->Attribute("name"));
			string zone = string(_race->Attribute("zone"));
			string food_transport_class = string(_race->Attribute("food_transport_class"));
			r.setRaceProperties(id, name, zone, food_transport_class);
			Game::AddRace(name, r);
		}
	}
	catch (...) 
	{
		std::cout << "An error occurred" << std::endl;
	}

	//------------------------------

	SetEnvironment("menu");
}

unsigned int Engine::GetEnvironment(void)
{
	return ENVIRONMENT;
}

void Engine::read_data(void) 
{
	/* images */

	vector<FileManager::file_info> imagesInfoList = FileManager::GetAllFilesNamesWithinSubfolders("assets\\ui", "png");

	for (int i = 0; i < imagesInfoList.size(); i++)
		Img()->addPath(imagesInfoList[i].name, imagesInfoList[i].path);
}

void Engine::ResetperipheralsInput(void)
{
	//Keyboard buttons
	Keyboard::ResetKeys();

	//Mouse buttons
	Mouse::LeftClick = false;
	Mouse::RightClick = false;
	Mouse::MiddleClick = false;
	Mouse::Release = false;
}

string Engine::GetCppVersion()
{
	string version;
	if (__cplusplus == 201703L) version = "C++17";
	else if (__cplusplus == 201402L) version = "C++14";
	else if (__cplusplus == 201103L) version = "C++11";
	else if (__cplusplus == 199711L) version = "C++98";
	else version = "pre-standard C++";
	return version;
}

Engine::Fps::Fps(void)
{
	this->nbFrames = 0;
	this->_Fps = 0;
	this->Mpfs = 0;
}

void Engine::Fps::SetLastTime(const double lastTime)
{
	this->lastTime = lastTime;
}

int Engine::Fps::GetFps(void)
{
	return this->_Fps;;
}

int Engine::Fps::GetMpfs(void)
{
	return this->Mpfs;
}

void Engine::Fps::Update(void) 
{
	this->currentTime = glfwGetTime();
	this->nbFrames++;
	if (this->currentTime - this->lastTime >= 1.0) 
	{ // If last prinf() was more than 1 sec ago
		this->_Fps = this->nbFrames;
		this->Mpfs = 1000 / this->nbFrames;
		this->lastTime += 1.0;
		this->nbFrames = 0;
	}
}

void Engine::Fps::SleepFps(void) 
{
	this->finalTime = glfwGetTime();
	if (this->finalTime - this->currentTime < 1.0 / 60.0) 
	{
		int x = 1000 * int(1.0 / 60.0 - (this->finalTime - this->currentTime));
		std::this_thread::sleep_for(std::chrono::milliseconds(x));
	}
}

Engine::Fps::~Fps(void) {}
