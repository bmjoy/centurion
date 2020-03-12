#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

class FileManager {
public:

	struct file_info {
		string name;
		string path;
	};

	//
	static string ReadFile(const char* fileLocation);

	// 
	static vector<string> GetAllFilesNamesWithinFolder(string folder, string type = "*");

	// 
	static vector<string> GetAllFoldersNamesWithinFolder(string folder);

	// 
	static vector<file_info> GetAllFilesNamesWithinSubfolders(string const &folder_name, string const &file_extension = "");

	//
	static string CurrentDateTime(const char* format);

	// 
	static bool CheckIfFolderExists(string folderPath);

	static void RemoveFile(string filePath);

};

#endif