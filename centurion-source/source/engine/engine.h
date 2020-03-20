#ifndef ENGINE_H
#define ENGINE_H

#include <ui.h>

#include <peripherics/camera.h>
#include <peripherics/mywindow.h>
#include <peripherics/mouse.h>
#include <peripherics/keyboard.h>

#ifndef MENU_ENV
#define MENU_ENV 0
#endif

#ifndef EDITOR_ENV
#define EDITOR_ENV 1
#endif

#ifndef STRATEGY_ENV
#define STRATEGY_ENV 2
#endif




namespace Engine {

	// Variables

	extern int ENVIRONMENT;
	extern bool ENGINE_RESET;

	// FPS Class

	class Fps {
	public:
		Fps();
		void SetLastTime(double lastTime) { this->lastTime = lastTime; }
		int GetFps(void) { return this->_Fps; }
		int GetMpfs(void) { return this->Mpfs; }
		void Update(void);
		void SleepFps(void);
		~Fps();
	private:
		int nbFrames, _Fps, Mpfs;
		double currentTime, lastTime, finalTime;
	};

    // TO-LUA Functions

	void GameClose();
	void SetEnvironment(string s);
	void PrintToConsole(string s);
	vector<string> *GetListOfFolders(string s);

	// Functions

	void Init (const char* exe_root);
	void Reset();
	int getEnvironment();
	int Launch();
	void read_data();
	void handleGlobalKeys();
	void ResetPeriphericsInput();

};

#endif