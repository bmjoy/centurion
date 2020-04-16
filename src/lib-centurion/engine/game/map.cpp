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
#include <classes/settlementArray.h>
#include <../hector/hector-lua.h>
#include <../hector/hector-misc.h>
#include <translationsTable.h>

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
			std::string MAP_NAME = "";
			bool IS_MAP_EDITED = false;
			bool IS_GRID_ENABLED = false;
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
				Game::Map::SetName(scenarioName); //This would be saved into a specific file among other properties
				Game::Map::MarkAsNotEdited();

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

		void Game::Map::DeleteScenario(const string scenarioName)
		{
			try
			{
				string scenarioPath = Folders::SCENARIOS + scenarioName;

				if (scenarioName == Game::Map::GetName())
				{
					Game::Map::UnableToDeleteScenario();
					return;
				}

				if (FileManager::CheckIfFolderExists(scenarioPath) == true)
				{
					FileManager::DeleteFolder(scenarioPath);
				}

				Logger::LogMessage msg = Logger::LogMessage("The scenario \"" + scenarioName + "\"" + " has been deleted!", "Info", "Game", "Map", "DeleteScenario");
				Logger::Info(msg);
			}
			catch (...)
			{
				Logger::LogMessage msg = Logger::LogMessage("An error occurred deleting the following scenario: \"" + scenarioName + "\"", "Info", "Game", "Map", "DeleteScenario");
				Logger::Error(msg);
				Engine::GameClose();
			}
		}

		void UnableToDeleteScenario(void)
		{
			Hector::EnableInfoBoxWindow();
			Hector::ExposeInfoBoxWindowText(TranslationsTable::GetWTranslation("EDITOR_unableToDelete"));
		}

		void Game::Map::SaveMapObjectsToXml(const string xmlPath)
		{
			try
			{
				ofstream mapobjs(xmlPath);

				mapobjs << "<mapObjects>\n";

				// SETTLEMENTS
				mapobjs << "\t<settlements>\n";
				
				std::vector<Settlement*>* settlList = SettlementArray::GetSettlementListPtr();
				for (int i = 0; i < settlList->size(); i++)
				{
					Settlement* settl = (*settlList)[i];

					mapobjs << "\t\t<settlement name=\"" << settl->GetSettlementName() <<"\" x=\"" << settl->GetPosition().x << "\" y=\""<< settl->GetPosition().y << "\" player=\"" << settl->GetPlayer() << "\" gold=\"" << settl->GetGold() << "\" food=\"" << settl->GetFood() << "\" pop=\"" << settl->GetPopulation() << "\" maxPop=\"" << settl->GetMaxPopulation() << "\">\n";
					std::vector<Building*> bldInSettlList = settl->GetBuildingsBelongToSettlement();
					for (auto bld : bldInSettlList)
					{
						int xOffset = int(settl->GetPosition().x - bld->GetPosition().x);
						int yOffset = int(settl->GetPosition().y - bld->GetPosition().y);
						mapobjs << "\t\t\t<building class=\"" << bld->GetClassName() << "\" idName=\"" << bld->GetIDName() << "\" xOffset=\"" << xOffset << "\" yOffset=\"" << yOffset << "\" name=\"" << bld->GetDisplayedName() << "\" healthPerc=\"" << bld->GetPercentHealth() << "\" />\n";
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

						GObject* thisBuilding = Game::CreateObject(className, posAbs.x, posAbs.y, 1);
						if (thisBuilding == nullptr) throw;

						std::string idName = _bld->Attribute("idName");
						thisBuilding->SetIDName(idName);
						std::string name = _bld->Attribute("name");
						thisBuilding->SetDisplayedName(name);
						unsigned int healthPerc = _bld->IntAttribute("healthPerc");
						thisBuilding->AsBuilding()->SetPercentHealth(healthPerc);
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
				GLItems::MapTerrain()->updateHeightsBuffer();
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
				GLItems::MapTerrain()->updateTextureBuffer();
			}
			catch (...)
			{
				Engine::GameClose();
			}
		}

		void SetName(const std::string name)
		{
			Game::Map::MAP_NAME = name;
		}

		const std::string GetName()
		{
			return Game::Map::MAP_NAME;
		}

		void MarkAsEdited(void)
		{
			IS_MAP_EDITED = true;
		}

		void MarkAsNotEdited(void)
		{
			IS_MAP_EDITED = false;
		}

		bool IsMapEdited(void)
		{
			return IS_MAP_EDITED;
		}

		bool Game::Map::IsGridEnabled(void)
		{
			return Game::Map::IS_GRID_ENABLED;
		}

		void Game::Map::EnableGrid(void)
		{
			Game::Map::IS_GRID_ENABLED = true;
		}

		void Game::Map::DisableGrid(void)
		{
			Game::Map::IS_GRID_ENABLED = false;
		}

		void Game::Map::Reset(void)
		{
			Game::Map::IS_GRID_ENABLED = false;
			Pass::ClearGrid();
			Game::Mapgen::ResetTexturesAndHeights();
			GLItems::MapTerrain()->updateHeightsBuffer();
			GLItems::MapTerrain()->updateTextureBuffer();
		}

		void Game::Map::CreateNoise(void)
		{
			Game::Mapgen::GenerateRandomHeights();
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
			if (Game::Map::IS_GRID_ENABLED && !tracing)
			{
				GLItems::MapGrid()->render();
			}
		}
	};
};
