#include "map.h"
#include "pass.h"

#include <fstream>

#include <logger.h>
#include <engine.h>
#include <file_manager.h>
#include <mapgen/mapgen.h>
#include <terrain.h>
#include <grid.h>
#include <pathfinding/pathfinding.h>

using namespace std;
using namespace glm;

bool Game::Map::Wireframe = false;

namespace Game
{
	namespace Map
	{
		// Private variables
		namespace 
		{
			bool isGridEnabled = false;
		}

		void Game::Map::LoadScenario(const string scenarioName)
		{
			try
			{
				string scenarioPath = "scenarios/" + scenarioName;

				Game::Map::LoadHeights(scenarioPath + "/heights");
				Game::Map::LoadTexture(scenarioPath + "/texture");
				Game::Map::LoadMapObjectsFromXml(scenarioPath + "/mapObjects.xml");

				//UpdateSettlementBuildings();
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred loading the following scenario: \"" + scenarioName + "\"", "Error", "Game", "Map", "LoadScenario");
				Logger::Error(msg);
				Engine::GameClose();
			}
		}

		void Game::Map::SaveScenario(const string scenarioName)
		{
			try
			{
				string scenarioPath = Folders::SCENARIOS + scenarioName;

				if (FileManager::CheckIfFolderExists(scenarioPath) == false)
				{
					FileManager::CreateFolder(scenarioPath);
				}

				Game::Map::SaveHeights(scenarioPath + "/heights");
				Game::Map::SaveTexture(scenarioPath + "/texture");
				Game::Map::SaveMapObjectsToXml(scenarioPath + "/mapObjects.xml");


				Logger::LogMessage msg = Logger::LogMessage("The scenario is saved with the following name: \"" + scenarioName + "\"", "Info", "Game", "Map", "SaveScenario");
				Logger::Info(msg);
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred creating the following scenario: \"" + scenarioName + "\"", "Info", "Game", "Map", "SaveScenario");
				Logger::Error(msg);
				Engine::GameClose();
			}
		}

		void Game::Map::SaveMapObjectsToXml(const string xmlPath)
		{
			try
			{
				// save buildings, settlements, decorations, units
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}

		void Game::Map::SaveHeights(const string path)
		{
			try
			{
				ofstream heightsFile(path);
				if (heightsFile.is_open()) {
					for (int i = 0; i < mapgen::nVertices * 4; i += 4)
						if (i == 0) {
							heightsFile << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
						}
						else {
							heightsFile << "," << mapgen::MapHeights()[i] << "," << mapgen::MapHeights()[i + 1] << "," << mapgen::MapHeights()[i + 2] << "," << mapgen::MapHeights()[i + 3];
						}
				}
				heightsFile.close();
			}
			catch (...)
			{
				Engine::GameClose();
			}

		}

		void Game::Map::SaveTexture(const string path)
		{
			try
			{
				ofstream textureFile(path);
				if (textureFile.is_open()) {
					for (int i = 0; i < mapgen::nVertices; i++)
						if (i == 0) {
							textureFile << mapgen::MapTextures()[i];
						}
						else {
							textureFile << "," << mapgen::MapTextures()[i];
						}
				}
				textureFile.close();
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}

		void Game::Map::LoadMapObjectsFromXml(const string xmlPath)
		{
			try
			{
				// load buildings, settlements, decorations, units
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}



		void Game::Map::LoadHeights(const string path)
		{
			try
			{
				fstream fin;
				fin.open(path);
				string line, number;
				getline(fin, line);
				stringstream s(line);
				int i = 0;
				while (getline(s, number, ',')) {
					mapgen::MapHeights()[i] = stof(number);
					i++;
				}
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}

		void Game::Map::LoadTexture(const string path)
		{
			try
			{
				fstream fin;
				fin.open(path);
				string line, number;
				getline(fin, line);
				stringstream s(line);
				int i = 0;
				while (getline(s, number, ',')) {
					mapgen::MapTextures()[i] = stof(number);
					i++;
				}
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}

		bool Game::Map::IsGridEnabled(void)
		{
			return Game::Map::isGridEnabled;
		}

		void Game::Map::EnableGrid(void)
		{
			Game::Map::isGridEnabled = true;
		}

		void Game::Map::DisableGrid(void)
		{
			Game::Map::isGridEnabled = false;
		}

		void Game::Map::Reset(void)
		{
			Game::Map::isGridEnabled = false;
			Pass::ClearGrid();
			mapgen::reset_map();
			GLItems::MapTerrain()->updateHeightsBuffer();
			GLItems::MapTerrain()->updateTextureBuffer();
		}

		void Game::Map::CreateNoise(void)
		{
			mapgen::generateRandomMap();
			GLItems::MapTerrain()->updateHeightsBuffer();

			stringstream ss;
			ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

			Logger::Info("Terrain has been generated!");
			Logger::Info(ss.str());
		}

		void Game::Map::UpdateGrid(void)
		{
			GLItems::MapGrid()->update();
		}

		void Game::Map::Render(const bool tracing)
		{
			GLItems::MapTerrain()->render(tracing);
			if (Game::Map::isGridEnabled && !tracing)
			{
				GLItems::MapGrid()->render();
			}
		}
	};
};
