#include "hector-lua.h"

#include <LUA/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <menu/menu.h>
#include <engine.h>
#include <picking.h>

#include <game/interface/editorMenuBar.h>
#include <game/interface/editorWindows.h>
#include <game/game.h>
#include <classes/object.h>

#include <ui.h>
#include <file_manager.h>


#include <GLFW/glfw3.h>

using namespace luabridge;

lua_State* Hector::L;
Hector::Console Hector::C;

void Hector::Initialize()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// global variables

	getGlobalNamespace(L).beginNamespace("Mouse").addVariable("RightClickID", &Picking::rightClickID).endNamespace();
	getGlobalNamespace(L).beginNamespace("Mouse").addVariable("LeftClickID", &Picking::leftClickID).endNamespace();

	getGlobalNamespace(L).beginNamespace("Mouse").addVariable("RightClick", &Engine::Mouse::RightClick).endNamespace();
	getGlobalNamespace(L).beginNamespace("Mouse").addVariable("LeftClick", &Engine::Mouse::LeftClick).endNamespace();

	// types 

	getGlobalNamespace(L)
		.beginClass<vector<string>>("vector_string")
		.endClass();

	getGlobalNamespace(L)
		.beginClass<vec3>("vec3")
		.endClass();

	// functions

	getGlobalNamespace(L).beginClass<Engine>("Engine")
		.addStaticFunction("GameClose", &Engine::GameClose)
		.addStaticFunction("PrintToConsole", &Engine::PrintToConsole)
		.addStaticFunction("SetEnvironment", &Engine::SetEnvironment)
		.addStaticFunction("GetListOfFolders", Engine::GetListOfFolders)
		.endClass();

	getGlobalNamespace(L).beginClass<EditorMenuBar>("EditorMenuBar")
		.addStaticFunction("ToggleMenu", &EditorMenuBar::ToggleEditorMenu)
		.endClass();

	getGlobalNamespace(L).beginClass<EditorWindows>("EditorWindows")
		.addStaticFunction("Open", &EditorWindows::OpenWindow)
		.endClass();

	getGlobalNamespace(L).beginClass<EditorWindows>("EditorWindows")
		.addStaticFunction("Open", &EditorWindows::OpenWindow)
		.endClass();

	getGlobalNamespace(L).beginClass<Menu>("Menu")
		.addStaticFunction("OpenMenuPage", &Menu::OpenMenuPage)
		.endClass();
	
	getGlobalNamespace(L).beginClass<GObject>("Object")
		.addFunction("GetClassName", &GObject::GetClassName)
		.endClass();

	getGlobalNamespace(L).beginClass<Game>("Game")
		.addStaticFunction("GetSelectedObject", &Game::GetSelectedObject)
		.addStaticFunction("IsObjectSelected", &Game::IsGameObjectSelected)
		.addStaticFunction("IsObjectNotNull", &Game::IsGameObjectNotNull)
		.addStaticFunction("CreateObject", &Game::CreateObject)
		.endClass();

	getGlobalNamespace(L)
		.beginClass<gui::TextList>("TextList")
		.addFunction("GetSelectedOption", &gui::TextList::GetSelectedOption)
		.addStaticFunction("GetListById", gui::TextList::GetTextListById)
		.addStaticFunction("UpdateListById", &gui::TextList::UpdateTextListById)
		.endClass();	
}

void Hector::ExecuteCommand(string cmd)
{
	if (cmd != "") {
		int r = luaL_dostring(L, cmd.c_str());

		if (r != LUA_OK) {
			std::string errormsg = lua_tostring(L, -1);
			Engine::PrintToConsole(errormsg);
		}
	}
}

bool Hector::ExecuteBooleanMethod(string cmd)
{
	bool boolean = false;

	if (cmd != "") {

		lua_getglobal(L, cmd.c_str());

		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "ERROR ON CALLING FUNCTION: " << lua_tostring(L, -1) << std::endl;
			boolean = false;
		}

		if (!lua_isboolean(L, -1)) {
			std::cout << "ERROR ON X RETURNING TYPE" << std::endl;
			boolean = false;
		}

		else {
			boolean = (bool)lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
	}
	return boolean;
}

void Hector::Console::Create()
{
	iframe = gui::Iframe("console");
	iframe.Create(30, 30, (int)Engine::myWindow::Width - 60, 30);
	txtinput = gui::TextInput();
	txtinput.create("", 35, 35, 200);
	isOpened = false;
}

void Hector::Console::Render()
{
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_F1)) {
		isOpened = !isOpened;
		txtinput.active(isOpened);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_F1, false);
	}
	if (isOpened) {		
		iframe.Render();
		txtinput.render();

		string cmd = txtinput.get_text();
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ENTER)) {
			if (cmd.size() > 0) {
				ExecuteCommand(cmd);
				txtinput.create("", 35, 35, 200);
				Engine::Keyboard::SetKeyStatus(GLFW_KEY_ENTER, false);
			}
		}
	}
}


void Hector::CreateConsole()
{
	C.Create();
}

void Hector::RenderConsole()
{
	C.Render();
}