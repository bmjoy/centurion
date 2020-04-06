/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>

namespace Game 
{
	namespace Map
	{
		/// <summary>
		/// This function loads a new scenario. It can throw an exception if an error occurs during the loading of the scenario.
		/// </summary>
		/// <param name="scenarioName">The name of the scanario (not the path!).</param>
		void LoadScenario(const std::string scenarioName);

		/// <summary>
		/// This function saves a scenario. 
		/// </summary>
		/// <param name="scenarioName">The name of the scanario (not the path!).</param>
		void SaveScenario(const std::string scenarioName);

		/// <summary>
		/// This function checks if the grid in the game map is enabled or not.
		/// </summary>
		/// <returns>True if it is enabled; false otherwise.</returns>
		bool IsGridEnabled(void);

		/// <summary>
		/// This function ebables the grid in the game map.
		/// </summary>
		void EnableGrid(void);

		/// <summary>
		/// This function resets the grid in the game map.
		/// </summary>
		void DisableGrid(void);

		/// <summary>
		/// This function resets the game map.
		/// </summary>
		void Reset(void);

		/// <summary>
		/// This function adds noise to the terrain of the game map.
		/// </summary>
		void CreateNoise(void);

		/// <summary>
		/// This function updates the grid of the game map.
		/// </summary>
		void UpdateGrid(void);

		/// <summary>
		/// This function performs the rendering of the game map.
		/// </summary>
		/// <param name="tracing">(???)</param>
		void Render(const bool tracing);

		/// <summary>
		/// (???) Forse da rivedere.
		/// This function saves al the objectes contained in the map on an XML file. 
		/// </summary>
		/// <param name="xmlPath">The path of the XML file.</param>
		void SaveMapObjectsToXml(const std::string xmlPath);

		/// <summary>
		/// This function saves on a file the heights of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		void SaveHeights(const std::string path);

		/// <summary>
		/// This function saves on a file the textures of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		void SaveTexture(const std::string path);

		/// <summary>
		/// (???) Da rivedere.
		/// This function loads from a file all the objects of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		void LoadMapObjectsFromXml(const std::string xmlPath);

		/// <summary>
		/// This function loads from a file the heights of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		void LoadHeights(const std::string path);

		/// <summary>
		/// This function loads from a file the textures of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		void LoadTexture(const std::string path);

		extern bool Wireframe;

	};
};