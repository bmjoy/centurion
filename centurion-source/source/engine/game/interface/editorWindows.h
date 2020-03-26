#pragma once

#include <ui.h>

#define MAX_NUMBER_OF_EDITOR_WINDOWS 20

/// <summary>
/// This class corresponds to all the windows of the editor.
/// </summary>
class EditorWindows 
{
public:
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
		void Create(string _luaOpeningScript, string _luaConditionScript, string _luaConditionFunction, gui::Iframe _iframe);
		/// <summary>
		/// This function performs the rendiring of a generic window.
		/// </summary>
		/// <param name="picking"></param>
		void Render(const bool picking);
	private:
		gui::Iframe iframe;
		bool isOpened;
		bool opening;
		string luaOpeningScript;
		string luaConditionScript;
		string luaConditionFunction;
	};

	/// <summary>
	/// This function clears all the windows of the editor.
	/// </summary>
	static void Clear(void);
	/// <summary>
	/// This function opens a specific window of the editor.
	/// </summary>
	/// <param name="id"></param>
	static void OpenWindow(const unsigned int id);
	/// <summary>
	/// This function creates an editor set of windows from an XML file. 
	/// </summary>
	static void Create(void);
	/// <summary>
	/// This function performs the rendering of all the windows of the editor.
	/// </summary>
	/// <param name="picking"></param>
	static void Render(const bool picking);
	~EditorWindows(void);
private:
	EditorWindows(void);	
	/// <summary>
	/// (???) Cosa succede se la finestra non esiste?
	/// This function adds a window.
	/// </summary>
	/// <param name="id">The id of the window.</param>
	/// <param name="win">The window.</param>
	static void AddWindow(const unsigned int id, EditorWindow* win);
	static array<EditorWindow*, MAX_NUMBER_OF_EDITOR_WINDOWS> listOfWindows;
};
