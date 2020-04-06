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
};
