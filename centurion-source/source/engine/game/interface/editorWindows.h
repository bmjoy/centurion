#ifndef EDITORWINDOWS_H
#define EDITORWINDOWS_H

#include <ui.h>

#define MAX_NUMBER_OF_EDITOR_WINDOWS 20

class EditorWindows {
public:

	class EditorWindow {
	public:
		bool IsOpened() { return isOpened; }
		void Open() { isOpened = true; opening = true; }
		void Close() { isOpened = false; }
		void Create(string _luaOpeningScript, string _luaConditionScript, string _luaConditionFunction, gui::Iframe _iframe);
		void Render(bool picking);
	private:
		gui::Iframe iframe;
		bool isOpened;
		bool opening;
		string luaOpeningScript;
		string luaConditionScript;
		string luaConditionFunction;
	};

	static void OpenWindow(int id);
	static void Create();
	static void Render(bool picking);

private:
	static void AddWindow(int id, EditorWindow* win) { listOfWindows[id] = win; }
	static array<EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> listOfWindows;
};

#endif 