#pragma once

#include <settings-xml.hxx>

using namespace std;

namespace glb {
	class Settings
	{
	public:
		Settings();
		bool ReadSettings();
		void SetDefaultSettings();
		void SaveXml();
		void SetCameraMaxZoom(float zoom) { cameraMaxZoom = zoom;  SettingsXML->cameraMaxZoom((int)zoom); }
		float GetCameraMaxZoom() { return cameraMaxZoom; }
		void SetCameraMovespeed(float speed) { cameraMovespeed = speed;  SettingsXML->cameraMovespeed((int)speed); }
		float GetCameraMovespeed() { return cameraMovespeed; }
		bool DebugIsActive() { return SettingsXML->debug(); }
		void SetFullScreen(bool full) { SettingsXML->fullScreen(full); }
		bool GetFullScreen() { return SettingsXML->fullScreen(); }
		void SetLanguage(string lang) { SettingsXML->language(lang); }
		string GetLanguage() { return SettingsXML->language(); }
		void SetWindowWidth(float x) { windowWidth = x;  SettingsXML->windowWidth(int(x)); }
		float GetWindowWidth() { return windowWidth; }
		void SetWindowHeight(float y) { windowHeight = y;  SettingsXML->windowHeight(int(y)); }
		float GetWindowHeight() { return windowHeight; }
		~Settings();
	private:
		string SettingsPath;
		std::auto_ptr<c_settings> SettingsXML;
		

		// just floats that are used a lot (increase speed avoiding conversions)
		float cameraMaxZoom, cameraMovespeed, windowWidth, windowHeight;
	};
};


