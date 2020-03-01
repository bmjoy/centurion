#ifndef SETTINGS_H
#define SETTINGS_H

#include <settings-xml.hxx>

using namespace std;

namespace glb {
	class Settings
	{
	public:
		static void Init();
		static bool ReadSettings();
		static void SetDefaultSettings();
		static void SaveXml();
		static void CameraMaxZoom(float zoom) { cameraMaxZoom = zoom;  SettingsXML->cameraMaxZoom((int)zoom); }
		static float CameraMaxZoom() { return cameraMaxZoom; }
		static void CameraMovespeed(float speed) { cameraMovespeed = speed;  SettingsXML->cameraMovespeed((int)speed); }
		static float CameraMovespeed() { return cameraMovespeed; }
		static bool DebugIsActive() { return SettingsXML->debug(); }
		static void FullScreen(bool full) { SettingsXML->fullScreen(full); }
		static bool FullScreen() { return SettingsXML->fullScreen(); }
		static void Language(string lang) { SettingsXML->language(lang); }
		static string Language() { return SettingsXML->language(); }
		static void SetWindowWidth(float x);  // get --> engine::myWindow::Width
		static void SetWindowHeight(float y); // get --> engine::myWindow::Height
	private:
		Settings();
		~Settings();
		static string SettingsPath;
		static auto_ptr<c_settings> SettingsXML;

		// just floats that are used a lot (increase speed avoiding conversions)
		static float cameraMaxZoom, cameraMovespeed, windowWidth, windowHeight;
	};
};

#endif