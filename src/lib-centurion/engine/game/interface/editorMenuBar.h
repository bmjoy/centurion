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

namespace EditorMenuBar 
{
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
		/// <param name="_optionsLuaCondition"></param>
		void Create(gui::Rectangle _titleBack, gui::SimpleText _titleText, std::vector<gui::Rectangle> _optionsBack, std::vector<gui::SimpleText> _optionsText, std::vector<std::string> _optionsLuaCondition);
		
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
		std::vector<std::string> optionsLuaCondition;
		std::vector<bool> optionsBooleans;
		bool isOpened;
	};

	/// <summary>
	/// This function checks if the editor menu is blocked or not.
	/// </summary>
	/// <returns>True if it is blocked; false otherwise.</returns>
	bool IsBlocked(void);

	/// <summary>
	/// This function enables/disables the editor menu.
	/// </summary>
	/// <param name="id">The id that corresponds to the editor menu.</param>
	void ToggleEditorMenu(const unsigned int id);

	/// <summary>
	/// This function creates an editor menu bar.
	/// </summary>
	void Create(void);

	/// <summary>
	/// This function performs the rendering of an editor menu bar.
	/// </summary>
	/// <param name="picking"></param>
	void Render(const bool picking);

	/// <summary>
	/// This function returns the editor menu bar height
	/// </summary>
	float GetHeight(void);

	/// <summary>
	/// This function hides the menu bar
	/// </summary>
	void Hide(void);

	/// <summary>
	/// This function displays the menu bar
	/// </summary>
	void Show(void);

	/// <summary>
	/// This function says if the menubar is hidden or not.
	/// </summary>
	bool IsHidden(void);

	/// <summary>
	/// This function says if the menubar is opened or not
	/// </summary>
	/// <returns></returns>
	bool IsOpened(void);

	/// <summary>
	/// This function makes the menubar unclickable
	/// </summary>
	void Block(void);

	/// <summary>
	/// This function unblocks the menubar
	/// </summary>
	void Unblock(void);

	/// <summary>
	/// This function adds a EditorMenu object to the editor menu bar.
	/// </summary>
	/// <param name="id">The id that corresponds to the editor menu bar.</param>
	/// <param name="menu">The editor menu</param>
	void AddMenu(const unsigned int id, EditorMenu* menu);

};
