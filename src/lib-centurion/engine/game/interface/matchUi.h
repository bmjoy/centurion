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
/// This STATIC class contains methods and variables of the UI of any match
/// and also the ObjectUI and the MatchMenu class
/// </summary>
class MatchUI {

public:


	/// <summary>
	/// This STATIC class contains the match Menu interface methods and variables
	/// </summary>
	class MatchMenu {
	
	public:
	
		/// <summary>
		/// Destructor
		/// </summary>
		~MatchMenu();

	private:
		/// <summary>
		/// Private constructor
		/// </summary>
		MatchMenu();
	};




	/// <summary>
	/// This class contains the object interface methods and variables
	/// It could take information from "GetSelectedObject" 
	/// </summary>
	class ObjectUI {
		
	};




	/// <summary>
	/// Create function
	/// </summary>
	static void Create();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="picking">Boolean: true = picking phase, false = normal rendering</param>
	static void Render(bool picking = false);

	/// <summary>
	/// Destruptor
	/// </summary>
	~MatchUI();
private:
	/// <summary>
	/// Private constructor
	/// </summary>
	MatchUI();

};