/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>

// N.B. IN FONDO PLAYERLIST

class Player
{
public:
	/// <summary>
	/// The constructor.
	/// </summary>
	Player(void);
	/// <summary>
	/// This function creates a new player.
	/// </summary>
	/// <param name="Id">The ID of the new player.</param>
	/// <param name="Team">The team to whom the new player belongs to.</param>
	/// <param name="Race">The race to whom the new player belongs to.</param>
	/// <param name="Col">The color of the new player.</param>
	void Create(const unsigned int Id, const unsigned int Team, const std::string Race, const glm::vec3 Col);
	/// <summary>
	/// This function sets a start point for the current player.
	/// </summary>
	/// <param name="point">The start point.</param>
	void SetStartPoint(const glm::vec2 point);
	/// <summary>
	/// This fnction returns the start point of the current player.
	/// </summary>
	/// <returns>The start point of the current player</returns>
	glm::vec2 GetStartPoint(void);
	/// <summary>
	/// (???) Da rivedere. Restituire i riferimenti ai dati privati di un oggetto e pericoloso (li rende in pratica NON privati).
	/// This function returns the color of the current player.
	/// </summary>
	/// <returns>The color of the current player</returns>
	glm::vec3 *GetPlayerColor(void);
	/// <summary>
	/// This function returns the team number to whom the current player belongs to.
	/// </summary>
	/// <returns>The team number to whom the current player belongs to.</returns>
	unsigned int GetPlayerTeam(void);
	/// <summary>
	/// This function returns the id of the current player.
	/// </summary>
	/// <returns>The id of the current player.</returns>
	unsigned int GetPlayerId(void);
	/// <summary>
	/// This function returns the race name with which the current player has started the game.
	/// </summary>
	/// <returns>The race name.</returns>
	std::string GetPlayerRace(void);
	~Player(void);
private:
	glm::vec3 color;
	unsigned int id;
	unsigned int team;
	std::string startRace;
	glm::vec2 startPoint;
};

extern std::array<Player, 8> playersList;
