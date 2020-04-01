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
/// This STATIC class contains methods and variables of the editor UI
/// </summary>
class EditorUI {
	
public:

	/// <summary>
	/// Create function
	/// </summary>
	static void Create();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="picking">Boolean: true = picking phase, false = normal rendering</param>
	static void Render(bool picking);

	/// <summary>
	/// Destructor
	/// </summary>
	~EditorUI();

private:
	/// <summary>
	/// Private constructor
	/// </summary>
	EditorUI();
};
