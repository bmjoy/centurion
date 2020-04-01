/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#define MAX_NUMBER_OF_PAGES 50

#include <ui.h>

class Menu 
{
public:
	/// <summary>
	/// A specific page of the menu.
	/// </summary>
	class MenuPage 
	{
	public:
		/// <summary>
		/// Public constructor. 
		/// </summary>
		MenuPage(void);
		/// <summary>
		/// This function return the name of the page.
		/// </summary>
		/// <returns>The name of the page.</returns>
		std::string GetPageName(void);
		/// <summary>
		/// This function creates a new page.
		/// </summary>
		/// <param name="name">The name to be assigned to the new page.</param>
		/// <returns>The ID assigned to the new page</returns>
		unsigned int Create(const std::string name);
		/// <summary>
		/// This function adds a new button to the page.
		/// </summary>
		/// <param name="btn">The button to add.</param>
		void AddButton(const gui::Button btn);
		/// <summary>
		/// This function adds a new image to the page.
		/// </summary>
		/// <param name="img">The image to add.</param>
		void AddImage(const gui::Image img);
		/// <summary>
		/// This function performs the rendering of the page of the menu.
		/// </summary>
		/// <param name="picking">Checks if it's the picking phase (true) or not (false).</param>
		void Render(const bool picking);
		~MenuPage(void);
	private:
		std::string pageName;
		std::vector<gui::Button> listOfButtons;
		std::vector<gui::Image> listOfImages;
	};


#pragma region TO-LUA Methods
	
	/// <summary>
	/// This function opens a specific menu page
	/// </summary>
	/// <param name="id">The ID of the menu page.</param>
	static void OpenMenuPage(const unsigned int id);

#pragma endregion

	/// <summary>
	/// This function checks if a menu was created.
	/// </summary>
	/// <returns>True if it was created; false otherwise.</returns>
	static bool IsCreated(void);
	/// <summary>
	/// This function adds a new page to the menu.
	/// </summary>
	/// <param name="id">The page ID.</param>
	/// <param name="mp">The menu page to add.</param>
	static void AddMenuPage(const unsigned int id, MenuPage* mp);
	/// <summary>
	/// (???) Da fare.
	/// </summary>
	static void Reset(void);
	/// <summary>
	/// This function removes all the pages from the menu.
	/// </summary>
	static void Clear(void);
	/// <summary>
	/// Thos function creates a new menu using a XML file. It throws an exception if an error occurs.
	/// </summary>
	static void Create(void);
	/// <summary>
	/// This function performs the picking assignment and the rendering of the menu.
	/// </summary>
	static void Run(void);
	/// <summary>
	/// The destructor. 
	/// </summary>
	~Menu(void);
private:
	Menu(void);
	/// <summary>
	/// This function performs the rendering of a specific menu page.
	/// </summary>
	/// <param name="id">The ID of the menu page.</param>
	/// <param name="picking">Checks if it's the picking phase (true) or not (false).</param>
	static void RenderPage(const unsigned int id, const bool picking);

	static std::array<MenuPage*, MAX_NUMBER_OF_PAGES> listOfPages;
	static bool isCreated;
	static int currentPageId;

};
