/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>



/// <summary>
/// This namespace contains main game folders constants.
/// </summary>
namespace Folders {

	extern std::string XML_SCHEMAS;
	extern std::string SCENARIOS;
	extern std::string GAME;
	extern std::string DATA;
	extern std::string INTERFACE_MENU;
	extern std::string INTERFACE_IFRAME;
	extern std::string INTERFACE_EDITOR;
};

/// <summary>
/// This namespace contains all methods and function for handling a file.
/// </summary>
namespace FileManager {

	struct file_info {
		std::string name;
		std::string path;
	};

	/// <summary>
	/// This function reads and returns a text file content.
	/// </summary>
	/// <param name="fileLocation">Text file path. Supports only char* variables or std::strings converted to c_str.</param>
	std::string ReadFile(const char* fileLocation);

	/// <summary>
	/// This function returns all files within a specified folder path.
	/// </summary>
	/// <param name="folder">Folder path. Supports std::strings.</param>
	/// <param name="type">The file extension required. Default value is "*", which means that every file extension it will be researched for.</param>
	std::vector<std::string> GetAllFilesNamesWithinFolder(std::string folder, std::string type = "*");

	/// <summary>
	/// This function returns all folders within a specified folder path.
	/// </summary>
	/// <param name="folder">Folder path. Supports std::strings.</param>
	std::vector<std::string> GetAllFoldersNamesWithinFolder(std::string folder);

	/// <summary>
	/// This function returns all files into all subfolders within a specified folder path.
	/// </summary>
	/// <param name="&folder_name">Folder path. Supports referenced std::strings.</param>
	/// <param name="&file_extension">The file extension required. Default value is "", which means that every file extension it will be researched for.</param> 
	std::vector<file_info> GetAllFilesNamesWithinSubfolders(std::string const &folder_name, std::string const &file_extension = "");

	/// <summary>
	/// This function returns current date and time.
	/// </summary>
	/// <param name="format">This parameter, which supports std::strings, can be customized by using the following variables (no specified requirement):
	/// %Y = Current year
	/// %m = Current month
	/// %d = Current day
	/// %H = Current hour
	/// %M = Current minute
	/// %S = Current seconds
	/// E.g. => %d\/%m\/%Y	01/01/1980.</param>
	std::string CurrentDateTime(const char* format);

	/// <summary>
	/// This boolean function checks if a precise folder exists in a specified folder path.
	/// </summary>
	/// <param name="folderPath">Folder path. Supports std::strings.</param>
	bool CheckIfFolderExists(std::string folderPath);

	/// <summary>
	/// This function removes a file into a specified path.
	/// </summary>
	/// <param name="filePath">File path. Supports std::strings.</param>
	void RemoveFile(std::string filePath);

	/// <summary>
	/// This function creates a folder into a specified path.
	/// </summary>
	/// <param name="folderPth">Folder path. Supports std::strings.</param>
	void CreateFolder(std::string folderPath);

	/// <summary>
	/// This function returns the relative file folder path.
	/// </summary>
	/// <param name="*path">Folder path. Supports only referenced char variables or std::strings converted to c_str.</param>
	std::string GetFileFolderPath(const char *path);
};

#endif