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
				string scenarioPath = Folders::SCENARIOS + scenarioName;

				Game::Map::LoadHeights(scenarioPath + "/heights");
				Game::Map::LoadTexture(scenarioPath + "/texture");
				Game::Map::LoadMapObjectsFromXml(scenarioPath + "/mapObjects.xml");
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

					mapobjs << "\t\t<settlement name=\"" << settl->GetSettlementName() <<"\" x=\"" << settl->GetPosition().x << "\" y=\""<< settl->GetPosition().y << "\">\n";
					std::vector<Building*> bldInSettlList = settl->GetBuildingsBelongToSettlement();
					for (auto bld : bldInSettlList)
					{
						int xOffset = int(settl->GetPosition().x - bld->GetPosition().x);
						int yOffset = int(settl->GetPosition().y - bld->GetPosition().y);
						mapobjs << "\t\t\t<building class=\"" << bld->GetClassName() << "\" xOffset=\"" << xOffset << "\" yOffset=\"" << yOffset << "\" />\n";
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
				throw;
			}
		}

		void Game::Map::LoadMapObjectsFromXml(const string xmlPath)
		{
			try
			{
				// load buildings, settlements, decorations, units
				tinyxml2::XMLDocument mapobjs;
				mapobjs.LoadFile(xmlPath.c_str());

				tinyxml2::XMLElement *_mapobjs = mapobjs.FirstChildElement("mapObjects");

				// SETTLEMENTS
				tinyxml2::XMLElement * _settls = _mapobjs->FirstChildElement("settlements");

				for (tinyxml2::XMLElement* _settl = _settls->FirstChildElement("settlement"); _settl != NULL; _settl = _settl->NextSiblingElement())
				{
					glm::vec3 pos = glm::vec3(_settl->IntAttribute("x"), _settl->IntAttribute("y"), 0);
					for (tinyxml2::XMLElement* _bld = _settl->FirstChildElement("building"); _bld != NULL; _bld = _bld->NextSiblingElement())
					{
						glm::vec3 posOffset = glm::vec3(_bld->IntAttribute("xOffset"), _bld->IntAttribute("yOffset"), 0);
						glm::vec3 posAbs = glm::vec3(pos.x - posOffset.x, pos.y - posOffset.y, 0);
						std::string className = _bld->Attribute("class");

						Game::CreateObject(className, posAbs.x, posAbs.y, 1);
					}
				}
			}
			catch (...)
			{
				throw;
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
