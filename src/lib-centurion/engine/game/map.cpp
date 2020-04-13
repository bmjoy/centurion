#include "map.h"
#include "pass.h"

#include <fstream>

#include <logger.h>
#include <engine.h>
#include <file_manager.h>
#include <gl_terrain.h>
#include <gl_grid.h>
#include <pathfinding/pathfinding.h>

#include <maths.hpp>

#include <game/game.h>
#include <game/strategy.h>
#include <player/player.h>

#include <gl_terrain.h>

#include <classes/building.h>

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
				ofstream mapobjs(xmlPath);

				mapobjs << "<mapObjects>\n";

				// SETTLEMENTS
				mapobjs << "\t<settlements>\n";
				
				std::vector<Settlement*>* settlList = Building::GetSettlementListPtr();
				for (int i = 0; i < settlList->size(); i++)
				{
					Settlement* settl = (*settlList)[i];

					mapobjs << "\t\t<settlement name=\"" << settl->GetSettlementName() <<"\">\n";
					std::vector<Building*> bldInSettlList = settl->GetBuildingsBelongToSettlement();
					for (auto bld : bldInSettlList)
					{
						mapobjs << "\t\t\t<building class=\"" << bld->GetClassName() << "\" />\n";
						bld->MarkAsSaved();
					}
					mapobjs << "\t\t</settlement>\n";
				}
				mapobjs << "\t</settlements>\n";


				mapobjs << "</mapObjects>\n";
				mapobjs.close();

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
					for (int i = 0; i < Game::Mapgen::GetNumberOfVertices() * 4; i += 4)
						if (i == 0) {
							heightsFile << Game::Mapgen::MapHeights()[i] << "," << Game::Mapgen::MapHeights()[i + 1] << "," << Game::Mapgen::MapHeights()[i + 2] << "," << Game::Mapgen::MapHeights()[i + 3];
						}
						else {
							heightsFile << "," << Game::Mapgen::MapHeights()[i] << "," << Game::Mapgen::MapHeights()[i + 1] << "," << Game::Mapgen::MapHeights()[i + 2] << "," << Game::Mapgen::MapHeights()[i + 3];
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
					for (int i = 0; i < Game::Mapgen::GetNumberOfVertices(); i++)
						if (i == 0) {
							textureFile << Game::Mapgen::MapTextures()[i];
						}
						else {
							textureFile << "," << Game::Mapgen::MapTextures()[i];
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
					Game::Mapgen::MapHeights()[i] = stof(number);
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
					Game::Mapgen::MapTextures()[i] = stof(number);
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
			Game::Mapgen::ResetTexturesAndHeights();
			GLItems::MapTerrain()->updateHeightsBuffer();
			GLItems::MapTerrain()->updateTextureBuffer();
		}

		void Game::Map::CreateNoise(void)
		{
			Game::Mapgen::generateRandomMap();
			GLItems::MapTerrain()->updateHeightsBuffer();

			stringstream ss;
			ss << "Min(z) = " << Game::Mapgen::GetMinZ() << "; Max(z) = " << Game::Mapgen::GetMaxZ();

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
