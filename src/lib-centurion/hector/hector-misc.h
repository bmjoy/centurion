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
	void PrCmd(const std::string s);

	/// <summary>
	/// This function toggles the wireframe view mode
	/// </summary>
	void ToggleWireframe(void);

	/// <summary>
	/// This function toggles the PASS grid view mode
	/// </summary>
	void ToggleGrid(void);

	/// <summary>
	/// This function returns the selected object
	/// </summary>
	GObject* Selo(void);

	void EnableMessageWindow(void);

	void ExposeMessageWindowText(std::string text);

	bool IsMessageWindowActive(void);

	std::string GetMessageWindowText(void);

	void ResetMessageWindow(void);

	void SetMessageWindowYesCmd(std::string cmd);

	void SetMessageWindowNoCmd(std::string cmd);

	void ExecuteYesCmdOfMessageWindow(void);

	void ExecuteNoCmdOfMessageWindow(void);
}

#endif