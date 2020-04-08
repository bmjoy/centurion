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
#include "terrain.h"
#include "image_sprite.h"
// 

#include <chrono>
#include <thread>

using namespace std;
using namespace glm;

namespace Engine 
{
	// Private variables
	namespace 
	{
		int ENVIRONMENT = MENU_ENV;
		std::vector<std::string> listOfFoldersTemp = std::vector<std::string>();
	};

	int Engine::Launch(void)
	{
		myWindow window = myWindow::GetInstance();
		GLItems::init();

		ObjectData::ReadDataClassesFromXml();
		Engine::read_data();

		GLItems::compile();
		GLItems::create();

		Camera::Init();
		Mouse::Create();

		DebugUI debugUI = DebugUI();
		debugUI.create();

		std::ostringstream ss;
		ss << glGetString(GL_VERSION);
		Logger::Info("Running OpenGL Version " + ss.str());

		Logger::Info("C++ Compiler Version: " + GetCppVersion());

		// Hector - Lua interpreter
		Hector::Initialize();
		Hector::CreateConsole();

		Fps::SetLastTime(glfwGetTime());

		while (myWindow::ShouldClose == false)
		{
			glfwPollEvents();
			Mouse::IsHolding();
			window.ClearBuffers();
			Fps::Update();
			Mouse::Control(window.get_mouse_x(), window.get_mouse_y());

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
				Game::Editor::Run();
			}

			// -------------- //

			// debug ui
			if (Settings::DebugIsActive)
			{
				debugUI.render(Fps::GetFps(), Fps::GetMpfs(), Unit::GetCounter());
			}

			Hector::RenderConsole();

			// mouse
			Mouse::Render();

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

			Fps::SleepFps();
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
			listOfFoldersTemp.clear();
			listOfFoldersTemp = FileManager::GetAllFoldersNamesWithinFolder(s);
			return &listOfFoldersTemp;
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

			GLItems::setMenuProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::Width, 0.0f, Engine::myWindow::Height, -100.0f, 100.0f));
			GLItems::setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));

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

		Engine::SetEnvironment("menu");
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
			GLItems::Img()->addPath(imagesInfoList[i].name, imagesInfoList[i].path);
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
		Mouse::ScrollBool = false;
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


	namespace Fps
	{
		namespace
		{
			int nbFrames = 0;
			int _Fps = 0;
			int Mpfs = 0;
			double currentTime = 0.0;
			double lastTime = 0.0;
			double finalTime = 0.0;
		};


		void Engine::Fps::SetLastTime(const double _lastTime)
		{
			lastTime = _lastTime;
		}

		int Engine::Fps::GetFps(void)
		{
			return _Fps;;
		}

		int Engine::Fps::GetMpfs(void)
		{
			return Mpfs;
		}

		void Engine::Fps::Update(void)
		{
			currentTime = glfwGetTime();
			nbFrames++;
			if (currentTime - lastTime >= 1.0)
			{ 
				// If last prinf() was more than 1 sec ago
				_Fps = nbFrames;
				Mpfs = 1000 / nbFrames;
				lastTime += 1.0;
				nbFrames = 0;
			}
		}

		void Engine::Fps::SleepFps(void)
		{
			finalTime = glfwGetTime();
			double delta = finalTime - currentTime;
			if (finalTime - currentTime < 1.0 / 60.0)
			{
				int sleepTime = int(1000.0 * (1.0 / 60.0 - (finalTime - currentTime)));
				std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			}
		}
	};

};