#pragma once

#include <ui.h>

#define MAX_NUMBER_OF_EDITOR_MENUS 10

class EditorMenuBar 
{
public:
	class EditorMenu 
	{
	public:
		/// <summary>
		/// This function checks if the editor menu is opened or not.
		/// </summary>
		/// <returns>True if it is opened; false otherwise.</returns>
		bool IsOpened(void);
		/// <summary>
		/// This function opens the editor menu.
		/// </summary>
		void Open(void);
		/// <summary>
		/// This function closes the editor menu.
		/// </summary>
		void Close(void);
		/// <summary>
		/// This function creates an editor menu.
		/// </summary>
		/// <param name="_titleBack"></param>
		/// <param name="_titleText"></param>
		/// <param name="_optionsBack"></param>
		/// <param name="_optionsText"></param>
		void Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, vector<gui::Rectangle> _optionsBack, vector<gui::SimpleText> _optionsText);
		/// <summary>
		/// This function performs the rendering of the editor menu.
		/// </summary>
		/// <param name="picking"></param>
		/// <param name="color"></param>
		void Render(const bool picking, const vec4 &color);
	private:
		gui::Rectangle titleBack;
		gui::SimpleText titleText;
		vector<gui::Rectangle> optionsBack;
		vector<gui::SimpleText> optionsText;
		bool isOpened;
	};

#pragma region TO-LUA Methods

	/// <summary>
	/// This function enables/disables the editor menu.
	/// </summary>
	/// <param name="id">The id that corresponds to the editor menu.</param>
	static void ToggleEditorMenu(const unsigned int id);

#pragma endregion

	/// <summary>
	/// This function creates an editor menu bar.
	/// </summary>
	static void Create(void);
	/// <summary>
	/// This function performs the rendering of an editor menu bar.
	/// </summary>
	/// <param name="picking"></param>
	static void Render(const bool picking);

private:
	/// <summary>
	/// This function adds a EditorMenu object to the editor menu bar.
	/// </summary>
	/// <param name="id">The id that corresponds to the editor menu bar.</param>
	/// <param name="menu">The editor menu</param>
	static void AddMenu(const unsigned int id, EditorMenu* menu);
	static unsigned int minPickingId, maxPickingId;
	static string font;
	static float height, width;
	static vec4 color;
	static gui::Rectangle topBar;
	static array<EditorMenu*, MAX_NUMBER_OF_EDITOR_MENUS> listOfMenus;
};
