#include "file_manager.h"

#include <logger.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <filesystem>
#include <engine.h>


using namespace std;
using namespace glm;

namespace Folders {

	string XML_SCHEMAS = "\\assets\\xml-schemas\\";
	string SCENARIOS = "\\scenarios\\";
	string GAME = "\\";
	string DATA = "\\assets\\data\\";
	string INTERFACE_MENU = "\\assets\\data\\interface\\menu\\";
	string INTERFACE_IFRAME = "\\assets\\data\\interface\\iframe\\";
	string INTERFACE_EDITOR = "\\assets\\data\\interface\\editor\\";

};


string FileManager::ReadFile(const char* fileLocation) {
	try {
		string content;
		ifstream fileStream(fileLocation, ios::in);
		if (!fileStream.is_open()) {
			stringstream ss;

			ss << "Failed to read " << fileLocation << "! File doesn't exist.";
			Logger::Warn(ss.str());
			return "";
		}
		string line = "";
		while (!fileStream.eof()) {
			getline(fileStream, line);
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}
	catch (...) {
		Engine::GameClose();
		throw;

	}
}

vector<string> FileManager::GetAllFilesNamesWithinFolder(string folder, string type) {
	try {

		vector<string> names;
		transform(type.begin(), type.end(), type.begin(), ::tolower);
		for (const auto & entry : std::filesystem::directory_iterator(folder)) {

			if (entry.is_directory()) continue;

			string fileName = entry.path().filename().string();
			if (type == "*") {
				names.push_back(fileName);
			}
			else {
				string fileExt = fileName.substr(fileName.find_last_of(".") + 1);
				transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
				if (fileExt == type) {
					names.push_back(fileName);
				}
			}
		}
		return names;
	}
	catch (...) {
		Engine::GameClose();
		throw;
	}
}

vector<string> FileManager::GetAllFoldersNamesWithinFolder(string folder) {
	try {
		vector<string> names;

		for (const auto & entry : std::filesystem::directory_iterator(folder)) {
			if (!entry.is_directory()) continue;

			string fileName = entry.path().filename().string();
			names.push_back(fileName);
		}
		return names;
	}
	catch (...) {
		Engine::GameClose();
		throw;
	}
}

vector<FileManager::file_info> FileManager::GetAllFilesNamesWithinSubfolders(string const &folder_name, string const &file_extension) {
	try {
		vector<file_info> output;
		string type = file_extension;
		transform(type.begin(), type.end(), type.begin(), ::tolower);
		for (const auto & entry : std::filesystem::recursive_directory_iterator(folder_name)) {

			if (entry.is_directory()) continue;

			file_info fi = file_info();
			fi.path = entry.path().string();

			string fullName = entry.path().filename().string();

			if (type == "*") {
				fi.name = fullName.substr(0, fullName.find_last_of("."));
				output.push_back(fi);
			}
			else {
				string fileExt = fullName.substr(fullName.find_last_of(".") + 1);
				transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
				
				if (fileExt == type) {
					fi.name = fullName.substr(0, fullName.find_last_of("."));
					output.push_back(fi);
				}
			}
		}
		return output;
	}
	catch (...) {
		Engine::GameClose();
		throw;

	}
}

string FileManager::CurrentDateTime(const char * format) {
	try {
		time_t     now = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&now);
		strftime(buf, sizeof(buf), format, &tstruct);
		return buf;
	}
	catch (...) {
		Engine::GameClose();
		throw;

	}
}

bool FileManager::CheckIfFolderExists(string folderPath) {
	try {
		struct stat info;
		if (stat(folderPath.c_str(), &info) == 0)
			return true;
		return false;
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred checking if this folder exists: \"" + folderPath + "\"", "Error", "Utils", "", "CheckIfFolderExists");
		Logger::Error(msg);
		Engine::GameClose();
		throw;

	}
}

void FileManager::RemoveFile(string filePath)
{
	try
	{
		std::filesystem::remove(filePath);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred deleting this following file: \"" + filePath + "\"", "Error", "Utils", "", "RemoveFile");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void FileManager::CreateFolder(string folderPath)
{
	try
	{
		std::filesystem::create_directories(folderPath);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred creating a folder with the following path: \"" + folderPath + "\"", "Error", "Utils", "", "CreateFolder");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

string FileManager::GetFileFolderPath(const char *path) {
	try
	{
		string path_str = (string)path;
		std::string::size_type pos = path_str.find_last_of("\\/");
		return path_str.substr(0, pos);
	}
	catch (const std::exception&)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the exe folder path", "Error", "Utils", "", "GetExeFolderPath");
		Logger::Error(msg);
		Engine::GameClose();
		throw;

	}

}
