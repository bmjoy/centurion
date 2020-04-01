#include "settings.h"

#include <logger.h>
#include <engine.h>
#include <translationsTable.h>
#include <iostream>
#include <fstream>
#include <file_manager.h>

#include <tinyxml2.h>

using namespace std;
using namespace glm;

// define private static variables
float Settings::cameraMaxZoom;
float Settings::cameraMovespeed;
float Settings::windowWidth;
float Settings::windowHeight;
bool Settings::FullScreen;
bool Settings::DebugIsActive;
string Settings::Language;
string Settings::SettingsPath;
//--- end definition

Settings::Settings() { }

void Settings::Init(void) 
{
	Language = "english";
	SettingsPath = "settings.xml";
	cameraMaxZoom = 20.f;
	cameraMovespeed = 10.f;
	windowWidth = 1366.f;
	windowHeight = 768.f;
	FullScreen = false;
}

void Settings::SetFolders(const string exe_folder_path)
{
	if (exe_folder_path.empty() == false) 
	{
		Folders::XML_SCHEMAS = exe_folder_path + Folders::XML_SCHEMAS;
		Folders::SCENARIOS = exe_folder_path + Folders::SCENARIOS;
		Folders::GAME = exe_folder_path + Folders::GAME;
		Folders::DATA = exe_folder_path + Folders::DATA;
		Folders::INTERFACE_MENU = exe_folder_path + Folders::INTERFACE_MENU;
		Folders::INTERFACE_IFRAME = exe_folder_path + Folders::INTERFACE_IFRAME;
		Folders::INTERFACE_EDITOR = exe_folder_path + Folders::INTERFACE_EDITOR;
	}
}

void Settings::ReadSettings(void)
{
	Deserialize();
	Serialize();
	TranslationsTable::ReadTranslationsTableXml(Language);

	Engine::myWindow::Width = windowWidth;
	Engine::myWindow::Height = windowHeight;
	Engine::Camera::MaxZoom = cameraMaxZoom;
	Engine::Camera::MovementSpeed = cameraMovespeed;
	Engine::myWindow::Ratio = windowWidth / windowHeight;
}

void Settings::SetCameraMovespeed(float speed) {
	cameraMovespeed = speed; 
	Engine::Camera::MovementSpeed = speed;
}

void Settings::SetCameraMaxZoom(float zoom) {
	cameraMaxZoom = zoom;
	Engine::Camera::MaxZoom = zoom;
}

void Settings::SetWindowSize(float width, float height) {
	windowWidth = width;
	windowHeight = height;
	Engine::myWindow::Width = width;
	Engine::myWindow::Height = height;
	Engine::myWindow::Ratio = width / height;
}

void Settings::ChangeLanguage(string lang)
{
	try {
		Language = lang;
		TranslationsTable::ReadTranslationsTableXml(lang);
		Serialize();
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred changing the language to \"" + lang + "\"", "Error", "", "Settings", "ChangeLanguage");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void Settings::Serialize(void)
{
	string debugStr = "false";
	if (DebugIsActive) debugStr = "true";
	string fullScreenStr = "false";
	if (FullScreen) fullScreenStr = "true";

	ofstream xmlFile(SettingsPath);
	if (xmlFile.is_open()) {
		xmlFile << 
			"<settings>" << endl <<
			"\t<windowWidth>" << windowWidth << "</windowWidth>" << endl <<
			"\t<windowHeight>" << windowHeight << "</windowHeight>" << endl <<
			"\t<cameraMovespeed>" << cameraMovespeed << "</cameraMovespeed>" << endl <<
			"\t<cameraMaxZoom>" << cameraMaxZoom << "</cameraMaxZoom>" << endl <<
			"\t<language>" << Language << "</language>" << endl <<
			"\t<debug>" << debugStr << "</debug>" << endl <<
			"\t<fullScreen>" << fullScreenStr << "</fullScreen>" << endl <<
			"</settings>" << endl;
	}
	xmlFile.close();
}

void Settings::Deserialize(void)
{
	tinyxml2::XMLDocument xmlFile;
	xmlFile.LoadFile(SettingsPath.c_str());

	try
	{
		string windowWidthStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("windowWidth")->GetText();
		windowWidth = stof(windowWidthStr);
	}
	catch (const std::exception&)
	{
		windowWidth = 1366.f;
		string msg = "An error occurred parsing the \"windowWidth\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string windowHeightStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("windowHeight")->GetText();
		windowHeight = stof(windowHeightStr);
	}
	catch (const std::exception&)
	{
		windowHeight = 768.f;
		string msg = "An error occurred parsing the \"windowHeight\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string cameraMovespeedStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("cameraMovespeed")->GetText();
		cameraMovespeed = stof(cameraMovespeedStr);
	}
	catch (const std::exception&)
	{
		cameraMovespeed = 10.f;
		string msg = "An error occurred parsing the \"cameraMovespeed\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string cameraMaxZoomStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("cameraMaxZoom")->GetText();
		cameraMaxZoom = stof(cameraMaxZoomStr);
	}
	catch (const std::exception&)
	{
		cameraMaxZoom = 20.f;
		string msg = "An error occurred parsing the \"cameraMaxZoom\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string languageStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("language")->GetText();
		Language = languageStr;
	}
	catch (const std::exception&)
	{
		Language = "english";
		string msg = "An error occurred parsing the \"Language\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string debugStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("debug")->GetText();
		DebugIsActive = (debugStr == "true");

	}
	catch (const std::exception&)
	{
		DebugIsActive = false;
		string msg = "An error occurred parsing the \"DebugIsActive\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	try
	{
		string fullScreenStr = (string)xmlFile.FirstChildElement("settings")->FirstChildElement("fullScreen")->GetText();
		FullScreen = (fullScreenStr == "true");
	}
	catch (const std::exception&)
	{
		FullScreen = false;
		string msg = "An error occurred parsing the \"FullScreen\"  value. Using default value.";
		Logger::LogMessage logmsg = Logger::LogMessage(msg, "Warn", "", "Settings", "Deserialize");
		Logger::Warn(logmsg);
	}
	
}

Settings::~Settings() {	}
