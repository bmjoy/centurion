#include "file_manager.h"

#include <logger.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <ctime>

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif
#include <Windows.h>

#include <direct.h>

namespace Folders {

	string XML_SCHEMAS = "\\assets\\xml-schemas\\";
	string SCENARIOS = "\\scenarios\\";
	string GAME = "\\";
	string DATA = "\\assets\\data\\";
	string INTERFACE_MENU = "\\assets\\data\\interface\\menu\\";

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
		throw;
	}
}

vector<string> FileManager::GetAllFilesNamesWithinFolder(string folder, string type) {
	try {
		vector<string> names;
		string search_path = folder + "/*.*";
		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					if (type == "*") {
						names.push_back(fd.cFileName);
					}
					else {
						string fullname = (string)fd.cFileName;
						string fileExt = fullname.substr(fullname.find_last_of(".") + 1);
						transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
						transform(type.begin(), type.end(), type.begin(), ::tolower);
						if (fileExt == type) {
							names.push_back(fd.cFileName);
						}
					}
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
		return names;
	}
	catch (...) {
		throw;
	}
}

vector<string> FileManager::GetAllFoldersNamesWithinFolder(string folder) {
	try {
		vector<string> names;
		WIN32_FIND_DATA findfiledata;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		char fullpath[MAX_PATH];
		GetFullPathName(folder.c_str(), MAX_PATH, fullpath, 0);
		string fp(fullpath);

		hFind = FindFirstFile((LPCSTR)(fp + "\\*").c_str(), &findfiledata);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if ((findfiledata.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY
					&& (findfiledata.cFileName[0] != '.'))
				{
					names.push_back(findfiledata.cFileName);
				}
			} while (FindNextFile(hFind, &findfiledata) != 0);
		}
		return names;
	}
	catch (...) {
		throw;
	}
}

vector<FileManager::file_info> FileManager::GetAllFilesNamesWithinSubfolders(string const &folder_name, string const &file_extension) {
	try {
		HANDLE finder;          // for FindFirstFile
		WIN32_FIND_DATA file;   // data about current file.
		priority_queue<string, vector<string>,
			greater<string> > dirs;
		dirs.push(folder_name); // start with passed directory 
		vector<file_info> output;
		do {
			string path = dirs.top();// retrieve directory to search
			dirs.pop();
			if (path[path.size() - 1] != '\\')  // normalize the name.
				path += "\\";
			string const fmask = "*";
			string mask = path + fmask;    // create mask for searching
			// First search for files:
			if (INVALID_HANDLE_VALUE == (finder = FindFirstFile(mask.c_str(), &file)))
				continue;
			do {
				if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					file_info fileInfo;
					fileInfo.name = file.cFileName;
					fileInfo.path = path + file.cFileName;
					if (file_extension == "") {
						fileInfo.name = fileInfo.name.substr(0, fileInfo.name.find_last_of("."));
						output.push_back(fileInfo);
					}
					else {
						string fullname = file.cFileName;
						string fileExt = fullname.substr(fullname.find_last_of(".") + 1);
						if (fileExt == file_extension) {
							fileInfo.name = fileInfo.name.substr(0, fileInfo.name.find_last_of("."));
							output.push_back(fileInfo);
						}
					}
				}
			} while (FindNextFile(finder, &file));
			FindClose(finder);
			// Then search for subdirectories:
			if (INVALID_HANDLE_VALUE == (finder = FindFirstFile((path + "*").c_str(), &file)))
				continue;
			do {
				if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (file.cFileName[0] != '.'))
					dirs.push(path + file.cFileName);
			} while (FindNextFile(finder, &file));
			FindClose(finder);
		} while (!dirs.empty());
		return output;
	}
	catch (...) {
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
		Logger::LogMessage msg = Logger::LogMessage("An error occurred checking if this folder exists: \"" + folderPath + "\"", "Utils", "", "CheckIfFolderExists");
		Logger::Error(msg);
		throw;
	}
}

void FileManager::RemoveFile(string filePath)
{
	try
	{
		DWORD res = DeleteFile(filePath.c_str());
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred deleting this following file: \"" + filePath + "\"", "Utils", "", "RemoveFile");
		Logger::Error(msg);
		throw;
	}
}

void FileManager::CreateFolder(string folderPath)
{
	try
	{
		_mkdir(folderPath.c_str());
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred creating a folder with the following path: \"" + folderPath + "\"", "Utils", "", "CreateFolder");
		Logger::Error(msg);
		throw;
	}
}

string FileManager::GetExeFolderPath(const char *path) {
	try
	{
		string path_str = (string)path;
		std::string::size_type pos = path_str.find_last_of("\\/");
		return path_str.substr(0, pos);
	}
	catch (const std::exception&)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred getting the exe folder path", "Utils", "", "GetExeFolderPath");
		Logger::Error(msg);
		throw;
	}
	
}
