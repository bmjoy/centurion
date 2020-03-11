#include "settings.h"
#include <engine/window.h>
#include <engine/camera.h>
#include <iostream>
#include <fstream>
#include <settings-xml.hxx>

namespace glb {

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
		SettingsPath = "Settings.xml";
		cameraMaxZoom = 20.f;
		cameraMovespeed = 10.f;
		windowWidth = 1366.f;
		windowHeight = 768.f;
		FullScreen = false;
	}

	void Settings::ReadSettings() {
		try {
			auto_ptr<c_settings> SettingsXML = c_settings_(SettingsPath);
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
		}
		catch (const xml_schema::exception & e) {
			std::cout << e << std::endl;
			SaveXml();
		}

		engine::myWindow::Width = windowWidth;
		engine::myWindow::Height = windowHeight;
		engine::Camera::MaxZoom = cameraMaxZoom;
		engine::Camera::MovementSpeed = cameraMovespeed;
		engine::myWindow::Ratio = windowWidth / windowHeight;
	}

	void Settings::SetCameraMovespeed(float speed) {
		cameraMovespeed = speed; 
		engine::Camera::MovementSpeed = speed;
	}

	void Settings::SetCameraMaxZoom(float zoom) {
		cameraMaxZoom = zoom;
		engine::Camera::MaxZoom = zoom;
	}

	void Settings::SetWindowSize(float width, float height) {
		windowWidth = width;
		windowHeight = height;
		engine::myWindow::Width = width;
		engine::myWindow::Height = height;
		engine::myWindow::Ratio = width / height;
	}

	void Settings::SaveXml()
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
		map[""].schema = "Settings.xsd";
		ofstream ofs(SettingsPath.c_str());
		c_settings_(ofs, settXML, map);
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
};