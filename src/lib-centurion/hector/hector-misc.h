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
	/// This function prints a string to the window cmd console.
	/// </summary>
	/// <param name="s">Text of the message that should appear on console. It supports only strings.</param>
	void PrCmd(const std::string s);

	/// <summary>
	/// This function toggles the wireframe view mode.
	/// </summary>
	void ToggleWireframe(void);

	/// <summary>
	/// This function toggles the PASS grid view mode.
	/// </summary>
	void ToggleGrid(void);

	/// <summary>
	/// This function returns the selected game object.
	/// </summary>
	/// <returns>Selected game object that should be returned if is not a null pointer</returns>
	GObject* Selo(void);

	/// <summary>
	/// This function enables and pops-up the Question Window.
	/// </summary>
	void EnableQuestionWindow(void);

	/// <summary>
	/// This function exposes the Question Window text message.
	/// </summary>
	/// <param name="text">Text of the message that should be contained into the Question Window. It supports only strings.</param> 
	void ExposeQuestionWindowText(std::string text);

	/// <summary>
	/// This boolean function returns true whenever a Question Window is shown.
	/// </summary>
	bool IsQuestionWindowActive(void);

	/// <summary>
	/// This function returns the message text of the Question Window.
	/// </summary>
	/// <returns>Text message of the Question Window, expressed in std::string</returns>
	std::string GetQuestionWindowText(void);

	/// <summary>
	/// This function peforms a reset of the Question Window, clearing all variables.
	/// </summary>
	void ResetQuestionWindow(void);

	/// <summary>
	/// This function sets the list of commands that should be executed when "Yes" is choosed.
	/// </summary>
	/// <param name="cmd">List of commands that should be executed when "Yes" is choosed. Commands must be separated with a semicolon. It supports only strings.</param> 
	void SetQuestionWindowYesCmd(std::string cmd);

	/// <summary>
	/// This function sets the list of commands that should be executed when "No" is choosed.
	/// </summary>
	/// <param name="cmd">List of commands that should be executed when "No" is choosed. Commands must be separated with a semicolon. It supports only strings.</param> 
	void SetQuestionWindowNoCmd(std::string cmd);

	/// <summary>
	/// This function executes the commands list when "Yes" is choosed.
	/// </summary>
	void ExecuteYesCmdOfQuestionWindow(void);

	/// <summary>
	/// This function executes the commands list when "No" is choosed.
	/// </summary>
	void ExecuteNoCmdOfQuestionWindow(void);
}

#endif