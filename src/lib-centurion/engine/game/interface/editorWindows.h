/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>

#ifndef MAX_NUMBER_OF_EDITOR_WINDOWS
#define MAX_NUMBER_OF_EDITOR_WINDOWS 20
#endif 


/// <summary>
/// This namespace contains all the windows of the editor.
/// </summary>
namespace EditorWindows 
{
	/// <summary>
	/// This class corresponds to a generic window of the editor.
	/// </summary>
	class EditorWindow 
	{
	public:
		/// <summary>
		/// This function checks if the generic windows is opened or not.
		/// </summary>
		/// <returns>True if it's opened; false otherwise.</returns>
		bool IsOpened(void);

		/// <summary>
		/// This function opens a generic window.
		/// </summary>
		void Open(void); 

		/// <summary>
		/// This function toggles a generic window.
		/// </summary>
		void Toggle(void);

		/// <summary>
		/// This function closes a generic window.
		/// </summary>
		void Close(void);

		/// <summary>
		/// This function clears a genric window.
		/// </summary>
		void Clear(void);

		/// <summary>
		/// This funtio creates a generic window.
		/// </summary>
		/// <param name="_luaOpeningScript"></param>
		/// <param name="_luaConditionScript"></param>
		/// <param name="_luaConditionFunction"></param>
		/// <param name="_iframe"></param>
		void Create(std::string _luaOpeningScript, std::string _luaConditionScript, std::string _luaConditionFunction, gui::Iframe _iframe);

		/// <summary>
		/// This function performs the rendiring of a generic window.
		/// </summary>
		/// <param name="picking">Checks if it's the picking phase.</param>
		void Render(const bool picking);

		/// <summary>
		/// This function returns the iframe pointer
		/// </summary>
		gui::Iframe* GetIframePtr(void) { return &iframe; }
	private:
		gui::Iframe iframe;
		bool isOpened;
		bool opening;
		std::string luaOpeningScript;
		std::string luaConditionScript;
		std::string luaConditionFunction;
	};

	/// <summary>
	/// This function clears all the windows of the editor.
	/// </summary>
	void Clear(void);

	/// <summary>
	/// This function opens a specific window of the editor.
	/// </summary>
	/// <param name="id"></param>
	void OpenWindow(const unsigned int id);

	/// <summary>
	/// This function closes a specific window of the editor.
	/// </summary>
	/// <param name="id"></param>
	void CloseWindow(const unsigned int id);

	/// <summary>
	/// This function toggles a specific window of the editor.
	/// </summary>
	/// <param name="id"></param>
	void ToggleWindow(const unsigned int id);

	/// <summary>
	/// This function creates an editor set of windows from an XML file. 
	/// </summary>
	void Create(void);

	/// <summary>
	/// This function performs the rendering of all the windows of the editor.
	/// </summary>
	/// <param name="picking"></param>
	void Render(const bool picking);

	/// <summary>
	/// This method hides the editor windows
	/// </summary>
	void Hide(void);
	
	/// <summary>
	/// This method shows the editor windows
	/// </summary>
	void Show(void);

	/// <summary>
	/// (DEPRECATED) This method returns true if any window is opened
	/// </summary>
	bool AnyWindowIsOpened(void);

	/// <summary>
	/// This method closes every opened window
	/// </summary>
	void CloseEveryWindow(void);

	/// <summary>
	/// This method returns true editor windows are hidden
	/// </summary>
	bool IsHidden(void);

	/// <summary>
	/// (???) Cosa succede se la finestra non esiste?
	/// This function adds a window.
	/// </summary>
	/// <param name="id">The id of the window.</param>
	/// <param name="win">The window.</param>
	void AddWindow(const unsigned int id, EditorWindow* win);

	/// <summary>
	/// This function returns a windows by ID.
	/// </summary>
	EditorWindow* GetWindowById(const unsigned int id);

	/// <summary>
	/// This function check if there ins any editor window opened.
	/// </summary>
	/// <returns>True or false.</returns>
	bool IsThereAnyWindowOpen(void);
};
