#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

using namespace std;

namespace FileManager {

	struct file_info {
		string name;
		string path;
	};

	//
	string ReadFile(const char* fileLocation);

	// 
	vector<string> GetAllFilesNamesWithinFolder(string folder, string type = "*");

	// 
	vector<string> GetAllFoldersNamesWithinFolder(string folder);

	// 
	vector<file_info> GetAllFilesNamesWithinSubfolders(string const &folder_name, string const &file_extension = "");

	//
	string CurrentDateTime(const char* format);

	// 
	bool CheckIfFolderExists(string folderPath);

	void RemoveFile(string filePath);

	void CreateFolder(string folderPath);
};

#endif