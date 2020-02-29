#include "settings.h"
#include <global>

namespace glb {

	Settings::Settings() {
		SettingsPath = "Settings.xml";
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
		c_settings settingsXMLtemp = c_settings(1366, 768, 10, 20, "english", false, false);
		xml_schema::namespace_infomap map;
		map[""].schema = "Settings.xsd";
		std::ofstream ofs(SettingsPath.c_str());
		c_settings_(ofs, settingsXMLtemp, map);
	}

	void Settings::SaveXml()
	{
		xml_schema::namespace_infomap map;
		map["gt"].name = "Settings.xml";
		map["gt"].schema = "Settings.xsd";
		std::ofstream ofs(SettingsPath.c_str());
		c_settings_(ofs, *SettingsXML, map);
	}

	Settings::~Settings() {	}
};