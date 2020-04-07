/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#define MAX_NUMBER_OF_PLAYERS 8
#define TOWNHALL_RADIUS 1875
#define OUTPOST_RADIUS 700

#include <ui.h>
#include <classes/object.h>

#include "map.h"
#include "race.h"
#include "minimap.h"
#include "selection_rectangle.h"

class GObject;
class Building;
class Decoration;
class Unit;

namespace Game
{
	/// <summary>
	/// Extern variables
	/// </summary>
	extern glm::mat4 projectionMatrix, viewMatrix;
	//extern bool isCreated;
	extern int numberOfPlayers;
	extern std::vector<glm::vec3> listOfColors;
	extern float cameraToX, cameraToY;
	extern GObject* selectedObject;

#pragma region TO-LUA Methods

	/// <summary>
	/// This functions returns the current selected object.
	/// </summary>
	/// <returns>The current selected object.</returns>
	/// 
	GObject* GetSelectedObject(void);
	/// <summary>
	/// (???) Metodi da rivedere/spostare?
	/// </summary>
	/// <returns></returns>
	bool IsGameObjectSelected(const unsigned int id);
	bool IsGameObjectNotNull(const unsigned int id);
	bool CreateObject(const std::string className, const float x, const float y, const unsigned int player);

#pragma endregion


#pragma region GameObjects List and Methods

	/// <summary>
	/// (???) Da rivedere.
	/// This function returns a list of all the indipendent buildings in the game.
	/// </summary>
	/// <returns>A list of buildings.</returns>
	std::vector<Building*> GetListOfIndipendentBuildings(void);

	//static void UpdateSettlementBuildings();

	/// <summary>
	/// This function sets the current selected object. 
	/// </summary>
	/// <param name="o">The selected object.</param>
	void SetSelectedObject(GObject* o);

#pragma endregion

#pragma region Race methods

	/// <summary>
	/// Metodi da rivedere. La gestione delle razze va rivista perche deve poter interagire anche con object.h.
	/// </summary>
	/// <param name="race_name"></param>
	/// <param name="r"></param>
	void AddRace(std::string race_name, Race r);
	std::vector<std::string> GetListOfRacesNames();
	Race* GetRace(std::string race_name);

#pragma endregion

#pragma region Colors methods

	/// <summary>
	/// (???) Da sistemare --> dovrebbe controllare che il colore non e gia presente.
	/// This function add a new color.
	/// </summary>
	/// <param name="col">A new color.</param>
	void AddColor(const glm::vec3 col);

	/// <summary>
	/// This function returns all the available colors.
	/// </summary>
	/// <returns>A list of color.</returns>
	std::vector<glm::vec3> GetListOfColors(void);

	/// <summary>
	/// (???) Cosa succede se il colore non e presente nella lista?
	/// This function returns a specific color.
	/// </summary>
	/// <param name="i">The index of the color.</param>
	/// <returns>A color.</returns>
	glm::vec3 GetColor(const unsigned int i);

	/// <summary>
	/// This function return the number of available colors.
	/// </summary>
	/// <returns>The number of available colors.</returns>
	unsigned int GetNumberOfColors(void);

#pragma endregion

	//(???) Tutti i metodi a seguire potrebbero essere da rivedere.
	void SetNumberOfPlayers(int n);
	void ResetGame(void);

	void GenerateSettlements(std::vector<glm::vec2> &locs);
	void GenerateOutposts(std::vector<glm::vec2> &locs);
	void GoToPointFromMinimap();
	void RenderObjectsPicking();
	void RenderObjects();
};
