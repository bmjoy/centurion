#ifndef MENU_H
#define MENU_H

#define MAX_NUMBER_OF_PAGES 50

#include <gui>

class Menu {

public:

	class MenuPage {
	public:
		MenuPage();
		string GetPageName() { return pageName; }
		int Create(string name);
		void AddButton(gui::Button btn);
		void AddImage(gui::Image img);
		void Render(bool picking);
		~MenuPage();
	private:
		string pageName;
		vector<gui::Button> listOfButtons;
		vector<gui::Image> listOfImages;
	};


#pragma region TO-LUA Methods
	
	static void OpenMenuPage(int id);

#pragma endregion

	static bool IsCreated() { return isCreated; }
	static void AddMenuPage(int id, MenuPage* mp) { listOfPages[id] = mp; }
	static void Reset();
	static void Clear();
	static void Create();
	static void Run();
	~Menu();
private:
	Menu();
	static void RenderPage(int id, bool picking);
	static array<MenuPage*, MAX_NUMBER_OF_PAGES> listOfPages;
	static bool isCreated;
	static int currentPageId;

};

#endif