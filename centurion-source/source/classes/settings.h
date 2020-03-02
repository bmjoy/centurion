#ifndef SETTINGS_H
#define SETTINGS_H

#include <settings-xml.hxx>

using namespace std;

namespace glb {
	class Settings
	{
	public:
		// settings
		static string Language;
		static bool DebugIsActive;
		static bool FullScreen;

		// methods
		static void Init();
		static void ReadSettings();
		static void SaveXml();
		static void SetCameraMovespeed(float speed);
		static void SetCameraMaxZoom(float zoom);
		static void SetWindowSize(float width, float height);
	private:
		Settings();
		~Settings();
		static void ParseInt(string name, string value, int* var);
		static void ParseFloat(string name, string value, float* var);
		static void ParseString(string name, string value, string* var);
		static void ParseBool(string name, string value, bool* var);
		static string SettingsPath;
		static float windowWidth;
		static float windowHeight;
		static float cameraMaxZoom;
		static float cameraMovespeed;
	};
};

#endif