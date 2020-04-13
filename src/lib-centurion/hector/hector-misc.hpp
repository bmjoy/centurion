/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef HECTOR_MISC_HPP
#define HECTOR_MISC_HPP

#include <game/map.h>
#include <game/game.h>
#include <logger.h>
#include <engine.h>
#include <string>

namespace Hector
{

	/// <summary>
	/// This function prints a string to the windows cmd console
	/// </summary>
	static void PrCmd(const std::string s)
	{
		std::cout << s << std::endl;
	}

	/// <summary>
	/// This function toggles the wireframe view mode
	/// </summary>
	static void ToggleWireframe(void)
	{
		if (Engine::GetEnvironment() == MENU_ENV) return;

		Game::Map::Wireframe = !Game::Map::Wireframe;
		Game::Map::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
	}

	/// <summary>
	/// This function toggles the PASS grid view mode
	/// </summary>
	static void ToggleGrid(void)
	{
		if (Engine::GetEnvironment() == MENU_ENV) return;

		if (Game::Map::IsGridEnabled()) Game::Map::DisableGrid();
		else Game::Map::EnableGrid();
		Game::Map::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
	}

	/// <summary>
	/// This function returns the selected object
	/// </summary>
	static GObject * Selo(void)
	{
		return Game::GetSelectedObject();
	}
}

#endif