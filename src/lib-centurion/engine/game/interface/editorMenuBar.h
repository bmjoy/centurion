/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

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
		void Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, std::vector<gui::Rectangle> _optionsBack, std::vector<gui::SimpleText> _optionsText);
		/// <summary>
		/// This function performs the rendering of the editor menu.
		/// </summary>
		/// <param name="picking">Checks if it's the picking phase.</param>
		/// <param name="color">The color to use.</param>
		void Render(const bool picking, const glm::vec4 &color);
	private:
		gui::Rectangle titleBack;
		gui::SimpleText titleText;
		std::vector<gui::Rectangle> optionsBack;
		std::vector<gui::SimpleText> optionsText;
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

	/// <summary>
	/// This function returns the editor menu bar height
	/// </summary>
	static float GetHeight(void) { return height; }

private:
	/// <summary>
	/// This function adds a EditorMenu object to the editor menu bar.
	/// </summary>
	/// <param name="id">The id that corresponds to the editor menu bar.</param>
	/// <param name="menu">The editor menu</param>
	static void AddMenu(const unsigned int id, EditorMenu* menu);
	static unsigned int minPickingId, maxPickingId;
	static std::string font;
	static float height, width;
	static glm::vec4 color;
	static gui::Rectangle topBar;
	static std::array<EditorMenu*, MAX_NUMBER_OF_EDITOR_MENUS> listOfMenus;
};
