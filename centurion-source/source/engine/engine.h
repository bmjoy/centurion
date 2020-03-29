/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>

#include <peripherals/camera.h>
#include <peripherals/mywindow.h>
#include <peripherals/mouse.h>
#include <peripherals/keyboard.h>

#ifndef MENU_ENV
#define MENU_ENV 0
#endif

#ifndef EDITOR_ENV
#define EDITOR_ENV 1
#endif

#ifndef STRATEGY_ENV
#define STRATEGY_ENV 2
#endif


namespace Engine 
{
	// Variables
	extern int ENVIRONMENT;
	extern bool ENGINE_RESET;

	/// <summary>
	/// This class handles a FPS.
	/// </summary>
	class Fps
	{
	public:
		/// <summary>
		/// Constructor. 
		/// </summary>
		Fps(void);
		/// <summary>
		/// (???) ?
		/// </summary>
		/// <param name="lastTime"></param>
		void SetLastTime(const double lastTime);
		/// <summary>
		/// This function returns a FPS.
		/// </summary>
		/// <returns>A FPS.</returns>
		int GetFps(void);
		/// <summary>
		/// This function returns a MPFS.
		/// </summary>
		/// <returns>A MPFS.</returns>
		int GetMpfs(void);
		/// <summary>
		/// This function performs the refresh of the screen.
		/// </summary>
		void Update(void);
		/// <summary>
		/// This function waits for a FPS.
		/// </summary>
		void SleepFps(void);
		/// <summary>
		/// The destructor. 
		/// </summary>
		~Fps(void);
	private:
		int nbFrames, _Fps, Mpfs;
		double currentTime, lastTime, finalTime;
	};

    // TO-LUA Functions

	/// <summary>
	/// This function closes the game.
	/// </summary>
	void GameClose(void);
	/// <summary>
	/// This functions sets an environment where the game is (for instance: editor, strategy, menu).
	/// </summary>
	/// <param name="s">The name of the enviroment. </param>
	void SetEnvironment(const string s);
	/// <summary>
	/// This functions displays a text to the console.
	/// </summary>
	/// <param name="s">The text you want to print to the console.</param>
	void PrintToConsole(const string s);
	/// <summary>
	/// This functions searches all the directories starting from a path.
	/// </summary>
	/// <param name="s">The path.</param>
	/// <returns>A list of folders; null if there are no folders.</returns>
	vector<string> *GetListOfFolders(const string s);

	// Functions
	/// <summary>
	/// Thois function initializes the game; it thorws an exception if it wasn't able to process the game.
	/// </summary>
	void Init (const char* exe_root);
	/// <summary>
	/// This function resets the engine. 
	/// </summary>
	void Reset(void);
	/// <summary>
	/// This functions returns the environment in which the game is (for instance editor, strategy, menu). 
	/// <returns>A value corresponding to the current environment.</returns>
	/// </summary>
	unsigned int GetEnvironment(void);
	/// <summary>
	/// (???) Da continuare?
	/// This function lauches the game.
	/// </summary>
	int Launch(void);
	/// <summary>
	/// (???) Da rivedere. 
	/// This function reads all the data needed for the game.
	/// </summary>
	void read_data(void);
	/// <summary>
	/// (???) Da completare?
	/// This function performs a specific action for each gloabal key (for instance: F10).
	/// </summary>
	void HandleGlobalKeys(void);
	/// <summary>
	/// This function resets mouse and keyboard restoring their default status. 
	/// </summary>
	void ResetperipheralsInput(void);

	/// <summary>
	/// This function returns the Cpp compiler version.
	/// </summary>
	string GetCppVersion();
};
