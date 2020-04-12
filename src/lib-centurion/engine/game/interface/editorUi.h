/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>

/// <summary>
/// This namespace contains methods and variables of the editor UI
/// </summary>
namespace EditorUI 
{
	/// <summary>
	/// Create function
	/// </summary>
	void Create();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="picking">Boolean: true = picking phase, false = normal rendering</param>
	void Render(bool picking);

	/// <summary>
	/// This function updates the info text shown in the top left of the screen
	/// </summary>
	/// <param name="infotext">A std::string indicating the new info text</param>
	void UpdateInfoText(std::wstring infotext);
};
