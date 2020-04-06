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
/// This namespace contains methods and variables of the UI of any match
/// and also the ObjectUI and the MatchMenu class
/// </summary>
namespace MatchUI {

	/// <summary>
	/// This STATIC class contains the match Menu interface methods and variables
	/// </summary>
	namespace MatchMenu {

	};




	/// <summary>
	/// This class contains the object interface methods and variables
	/// It could take information from "GetSelectedObject" 
	/// </summary>
	namespace ObjectUI {
		
	};




	/// <summary>
	/// Create function
	/// </summary>
	void Create();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="picking">Boolean: true = picking phase, false = normal rendering</param>
	void Render(bool picking = false);

};