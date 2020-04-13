/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <header.h>
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
	/// <summary>
	/// This namespace handles the game FPS.
	/// </summary>
	namespace Fps
	{
		/// <summary>
		/// This function determines the last time a frame was calculated, by using in-game seconds.
		/// </summary>
		/// <param name="lastTime">This parameter sets the current moment. It's recommended to use glfwGetTime() function</param>
		void SetLastTime(const double _lastTime);

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
	};

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
	/// This functions searches all the directories starting from a path.
	/// </summary>
	/// <param name="s">The path.</param>
	/// <returns>A list of folders; null if there are no folders.</returns>
	std::vector<std::string> *GetListOfFolders(const std::string s);

	/// <summary>
	/// This function initializes the game; it returns false if it wasn't able to process the game.
	/// </summary>
	bool Initialize (const char* exe_root);

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
	void InitializeImages(void);

	/// <summary>
	/// This function resets mouse and keyboard restoring their default status. 
	/// </summary>
	void ResetperipheralsInput(void);

	/// <summary>
	/// This function returns the Cpp compiler version.
	/// </summary>
	std::string GetCppVersion();

	/// <summary>
	/// This function returns the Opengl version.
	/// </summary>
	std::string GetOpenglVersion();
};
