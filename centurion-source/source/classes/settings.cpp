#include "settings.h"
#include <iostream>
#include <fstream>

namespace glb {

	// define private static variables
	float Settings::cameraMaxZoom;
	float Settings::cameraMovespeed;
	float Settings::windowWidth;
	float Settings::windowHeight;
	string Settings::SettingsPath;
	Settings Settings::mysettings;
	auto_ptr<c_settings> Settings::SettingsXML;
	//--- end definition

	Settings::Settings() {
		SettingsPath = "Settings.xml";
		cameraMaxZoom = 20;
		cameraMovespeed = 10;
		windowWidth = 1366;
		windowHeight = 768;
	}

	void Settings::Init() {
		mysettings = Settings();
	}

	bool Settings::ReadSettings() {
		try {
			SettingsXML = c_settings_(SettingsPath);
		}
		catch (const xml_schema::exception & e) {
			std::cout << e << std::endl;
			return false;
		}
		
		cameraMaxZoom = float(SettingsXML->cameraMaxZoom());
		cameraMovespeed = float(SettingsXML->cameraMovespeed());
		windowWidth = float(SettingsXML->windowWidth());
		windowHeight = float(SettingsXML->windowHeight());

		return true;
	}

	void Settings::SetDefaultSettings()
	{
		c_settings settingsXMLtemp = c_settings(
			(const c_settings::windowWidth_type)windowWidth, 
			(const c_settings::windowHeight_type)windowHeight,
			(const c_settings::cameraMovespeed_type)cameraMovespeed,
			(const c_settings::cameraMaxZoom_type)cameraMaxZoom,
			(const c_settings::language_type)"english",
			(const c_settings::debug_type)false, 
			(const c_settings::fullScreen_type)false
		);

		xml_schema::namespace_infomap map;
		map[""].schema = "Settings.xsd";
		ofstream ofs(SettingsPath.c_str());
		c_settings_(ofs, settingsXMLtemp, map);
	}

	void Settings::SaveXml()
	{
		xml_schema::namespace_infomap map;
		map[""].schema = "Settings.xsd";
		ofstream ofs(SettingsPath.c_str());
		c_settings_(ofs, *SettingsXML, map);
	}

	Settings::~Settings() {	}
};