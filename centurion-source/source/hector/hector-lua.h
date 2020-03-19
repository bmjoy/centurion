#ifndef HECTOR_LUA_H
#define HECTOR_LUA_H

#include <ui.h>

using namespace std;
struct lua_State;

class Hector {
public:

	class Console {
	public:
		bool IsOpened() { return isOpened; }
		void Create();
		void Render();
	private:
		gui::Iframe iframe;
		gui::TextInput txtinput;
		bool isOpened;
	};

	static bool ConsoleIsActive() { return C.IsOpened(); }
	static void CreateConsole();
	static void RenderConsole();
	static void Initialize();
	static void ExecuteCommand(string cmd);
	static bool ExecuteBooleanMethod(string cmd);

private:
	static lua_State* L;
	static Console C;
};

#endif