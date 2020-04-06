#include "map.h"

#include <fstream>

#include <logger.h>
#include <engine.h>
#include <file_manager.h>
#include <mapgen/mapgen.h>
#include <terrain.h>
#include <grid.h>

using namespace std;
using namespace glm;

bool Game::Map::Wireframe = false;
bool Game::Map::isGridEnabled;

Game::Map::Map(void) {}

void Game::Map::LoadScenario(const string scenarioName)
{
	try
	{
		string scenarioPath = "scenarios/" + scenarioName;

		LoadHeights(scenarioPath + "/heights");
		LoadTexture(scenarioPath + "/texture");
		LoadMapObjectsFromXml(scenarioPath + "/mapObjects.xml");

		//UpdateSettlementBuildings();
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred loading the following scenario: \"" + scenarioName + "\"", "", "Game::Map", "LoadScenario");
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

		SaveHeights(scenarioPath + "/heights");
		SaveTexture(scenarioPath + "/texture");
		SaveMapObjectsToXml(scenarioPath + "/mapObjects.xml");


		Logger::LogMessage msg = Logger::LogMessage("The scenario is saved with the following name: \"" + scenarioName + "\"", "", "Game::Map", "SaveScenario");
		Logger::Info(msg);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred creating the following scenario: \"" + scenarioName + "\"", "", "Game::Map", "SaveScenario");
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
	isGridEnabled = false;
	mapgen::reset_map();
	MapGrid()->reset();
	MapTerrain()->updateHeightsBuffer();
	MapTerrain()->updateTextureBuffer();
}

void Game::Map::CreateNoise(void)
{
	mapgen::generateRandomMap();
	MapTerrain()->updateHeightsBuffer();

	stringstream ss;
	ss << "Min(z) = " << mapgen::minZ << "; Max(z) = " << mapgen::maxZ;

	Logger::Info("Terrain has been generated!");
	Logger::Info(ss.str());
}

void Game::Map::UpdateGrid(void)
{
	MapGrid()->update();
}

void Game::Map::Render(const bool tracing)
{
	MapTerrain()->render(tracing);
	if (isGridEnabled && !tracing)
	{
		MapGrid()->render();
	}
}

Game::Map::~Map(void) {}