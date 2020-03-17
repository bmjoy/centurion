#include "settings.h"

#include <logger.h>
#include <engine.h>
#include <translationsTable.h>
#include <iostream>
#include <fstream>
#include <settings-xml.hxx>
#include <file_manager.h>

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

void Settings::Init() {
	Language = "english";
	SettingsPath = "settings.xml";
	cameraMaxZoom = 20.f;
	cameraMovespeed = 10.f;
	windowWidth = 1366.f;
	windowHeight = 768.f;
	FullScreen = false;
}

void Settings::SetFolders(string exe_folder_path)
{
	if (exe_folder_path.empty() == false) {
		Folders::XML_SCHEMAS = exe_folder_path + Folders::XML_SCHEMAS;
		Folders::SCENARIOS = exe_folder_path + Folders::SCENARIOS;
		Folders::GAME = exe_folder_path + Folders::GAME;
		Folders::DATA = exe_folder_path + Folders::DATA;
		Folders::INTERFACE_MENU = exe_folder_path + Folders::INTERFACE_MENU;
		Folders::INTERFACE_IFRAME = exe_folder_path + Folders::INTERFACE_IFRAME;
		Folders::INTERFACE_EDITOR = exe_folder_path + Folders::INTERFACE_EDITOR;
	}
}

void Settings::ReadSettings() {
	try {
		xml_schema::properties props;
		props.no_namespace_schema_location(Folders::XML_SCHEMAS + "settings.xsd");
		auto_ptr<c_settings> SettingsXML = c_settings_(Folders::GAME + SettingsPath, 0, props);
		c_settings::setting_const_iterator it;
		for (it = SettingsXML->setting().begin(); it != SettingsXML->setting().end(); it++) {
			try {
				string name = it->name();
				string value = it->value();

				if (name == "windowWidth") ParseFloat(name, value, &windowWidth);
				if (name == "windowHeight") ParseFloat(name, value, &windowHeight);
				if (name == "cameraMovespeed") ParseFloat(name, value, &cameraMovespeed);
				if (name == "cameraMaxZoom") ParseFloat(name, value, &cameraMaxZoom);
				if (name == "language") ParseString(name, value, &Language);
				if (name == "debug") ParseBool(name, value, &DebugIsActive);
				if (name == "fullScreen") ParseBool(name, value, &FullScreen);
			}
			catch (...) {
				std::cout << "An error occurred parsing the XML file. Using default values.";
				SaveXml();
			}
		}
		TranslationsTable::ReadTranslationsTableXml(Language);
	}
	catch (const xml_schema::exception & e) {
		std::cout << e << std::endl;
		SaveXml();
	}

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
		SaveXml();
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred changing the language to \"" + lang + "\"", "Error", "", "Settings", "ChangeLanguage");
		Logger::Error(msg);
		throw;
	}
}

void Settings::SaveXml()
{
	try
	{
		c_settings settXML = c_settings();
		vector<pair<string, string>> settings_;

		// Define strings for the XML
		ostringstream windowWidthStr, windowHeightStr, cameraMovespeedStr, cameraMaxZoomStr;
		windowWidthStr << (int)windowWidth;
		windowHeightStr << (int)windowHeight;
		cameraMovespeedStr << (int)cameraMovespeed;
		cameraMaxZoomStr << (int)cameraMaxZoom;
		string debugStr = "false";
		if (DebugIsActive) debugStr = "true";
		string fullScreenStr = "false";
		if (FullScreen) fullScreenStr = "true";

		settings_.push_back({ "windowWidth", windowWidthStr.str() });
		settings_.push_back({ "windowHeight", windowHeightStr.str() });
		settings_.push_back({ "cameraMovespeed", cameraMovespeedStr.str() });
		settings_.push_back({ "cameraMaxZoom", cameraMaxZoomStr.str() });
		settings_.push_back({ "language", Language });
		settings_.push_back({ "debug", debugStr });
		settings_.push_back({ "fullScreen", fullScreenStr });

		for (int i = 0; i < settings_.size(); i++) {
			setting xml_setting = setting(settings_[i].first, settings_[i].second);
			settXML.setting().push_back(xml_setting);
		}

		xml_schema::namespace_infomap map;
		map[""].schema = "";
		ofstream ofs(Folders::GAME + SettingsPath);
		c_settings_(ofs, settXML, map);
	}
	catch (...)
	{
		Logger::LogMessage msg = Logger::LogMessage("An error occurred saving \"Settings.XML\"", "Error", "", "Settings", "ChangeLanguage");
		Logger::Error(msg);
		throw;
	}
}

void Settings::ParseInt(string name, string value, int* var) {
	try {
		(*var) = std::stoi(value);
	}
	catch (...) {
		std::cout << "An error occurred parsing the \"" + name + "\"  value. Using default value.";
	}
}
void Settings::ParseFloat(string name, string value, float* var) {
	try {
		(*var) = std::stof(value);
	}
	catch (...) {
		std::cout << "An error occurred parsing the \"" + name + "\"  value. Using default value.";
	}
}
void Settings::ParseString(string name, string value, string* var) {
	try {
		(*var) = (value);
	}
	catch (...) {
		std::cout << "An error occurred parsing the \"" + name + "\"  value. Using default value.";
	}
}
void Settings::ParseBool(string name, string value, bool* var) {
	try {
		(*var) = (value == "true");
	}
	catch (...) {
		std::cout << "An error occurred parsing the \"" + name + "\"  value. Using default value.";
	}
}

Settings::~Settings() {	}