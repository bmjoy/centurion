/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <string>

class Settings
{
public:
	// settings
	static std::string Language;
	static bool DebugIsActive;
	static bool FullScreen;

	// methods
	/// <summary>
	/// This function initializes the settingsof the game. 
	/// </summary> 
	static void Init(void);
	/// <summary>
	/// This function reads the setting, using the Serialize function.
	/// It could throw an exception if an erroroccurs during the reading of the file. 
	/// </summary>
	static void ReadSettings(void);
	/// <summary>
	/// This functions sets all the folders from which to read the data of the game.
	/// </summary>
	/// <param name="exe_folder_path">The home path.</param>
	static void SetFolders(std::string exe_folder_path);
	
	static void SetCameraMovespeed(float speed);
	static void SetCameraMaxZoom(float zoom);
	static void SetWindowSize(float width, float height);
	static void ChangeLanguage(std::string lang);
	const static std::string GetGameNameStr(void) { return "Centurion"; }
	const static char* GetGameName(void) { return "Centurion"; }
private:
	Settings();
	~Settings();

	/// <summary>
	/// This function reads all the settings from an XML file.
	/// It could be throws an exception if an error occurs during the process 
	/// </summary>
	static void Serialize(void);

	/// <summary>
	/// This function saves all the settings into an XML file.
	/// It could be throws an exception if an error occurs during the process 
	/// </summary>
	static void Deserialize(void);

	static std::string SettingsPath;
	static float windowWidth;
	static float windowHeight;
	static float cameraMaxZoom;
	static float cameraMovespeed;
};
