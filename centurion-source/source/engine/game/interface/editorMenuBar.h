#ifndef EDITORMENUBAR_H
#define EDITORMENUBAR_H

#include <ui.h>

#define MAX_NUMBER_OF_EDITOR_MENUS 10

class EditorMenuBar {
public:
	class EditorMenu {
	public:
		bool IsOpened() { return isOpened; }
		void Open() { isOpened = true; }
		void Close() { isOpened = false; }
		void Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, vector<gui::Rectangle> _optionsBack, vector<gui::SimpleText> _optionsText);
		void Render(bool picking, vec4 &color);
	private:
		gui::Rectangle titleBack;
		gui::SimpleText titleText;
		vector<gui::Rectangle> optionsBack;
		vector<gui::SimpleText> optionsText;
		bool isOpened;
	};

#pragma region TO-LUA Methods

	static void ToggleEditorMenu(int id);

#pragma endregion


	static void Create();
	static void Render(bool picking);

private:
	static unsigned int minPickingId, maxPickingId;
	static void AddMenu(int id, EditorMenu* menu) { listOfMenus[id] = menu; }
	static string font;
	static float height, width;
	static vec4 color;
	static gui::Rectangle topBar;
	static array<EditorMenu*, MAX_NUMBER_OF_EDITOR_MENUS> listOfMenus;
};

#endif 