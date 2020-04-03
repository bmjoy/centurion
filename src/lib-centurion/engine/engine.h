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
	#pragma region Variables
	extern int ENVIRONMENT;

	/// <summary>
	/// This class handles the game FPS.
	/// </summary>
	class Fps
	{
	public:
		Fps(void);

		/// <summary>
		/// This function determines the last time a frame was calculated, by using in-game seconds.
		/// </summary>
		/// <param name="lastTime">This parameter sets the current moment. It's recommended to use glfwGetTime() function</param>
		void SetLastTime(const double lastTime);

		/// <summary>
		/// This function returns the game Frames Per Second.
		/// </summary>
		/// <returns>A FPS.</returns>
		int GetFps(void);

		/// <summary>
		/// This function returns milliseconds to process each frame.
		/// </summary>
		/// <returns>A MPFS.</returns>
		int GetMpfs(void);

		/// <summary>
		/// This function performs the refresh of the screen.
		/// </summary>
		void Update(void);

		/// <summary>
		/// This function pauses frames execution to lock in game FPS.
		/// </summary>
		void SleepFps(void);

		~Fps(void);
	private:
		int nbFrames, _Fps, Mpfs;
		double currentTime, lastTime, finalTime;
	};
	#pragma endregion

	#pragma region TO-LUA Functions

	/// <summary>
	/// This function closes the game.
	/// </summary>
	void GameClose(void);
	
	/// <summary>
	/// This functions sets an environment where the game is (for instance: editor, strategy, menu).
	/// </summary>
	/// <param name="s">The name of the enviroment.</param>
	void SetEnvironment(const std::string s);
	
	/// <summary>
	/// This functions displays a text to the console.
	/// </summary>
	/// <param name="s">The text you want to print to the console.</param>
	void PrintToConsole(const std::string s);

	/// <summary>
	/// This functions searches all the directories starting from a path.
	/// </summary>
	/// <param name="s">The path.</param>
	/// <returns>A list of folders; null if there are no folders.</returns>
	std::vector<std::string> *GetListOfFolders(const std::string s);

	extern std::vector<std::string> listOfFoldersTemp;
	#pragma endregion

	#pragma region Functions
	/// <summary>
	/// Thois function initializes the game; it thorws an exception if it wasn't able to process the game.
	/// </summary>
	void Init (const char* exe_root);

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
	/// This function resets mouse and keyboard restoring their default status. 
	/// </summary>
	void ResetperipheralsInput(void);

	/// <summary>
	/// This function returns the Cpp compiler version.
	/// </summary>
	std::string GetCppVersion();
	#pragma endregion
};
