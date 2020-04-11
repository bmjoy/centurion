#include "hector-lua.h"

#include <LUA/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <menu/menu.h>
#include <game/editor.h>
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
using namespace std;
using namespace glm;

lua_State* Hector::L;
Hector::Console Hector::C;

void Hector::Initialize()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// no namespace functions

	getGlobalNamespace(L)
		.addFunction("PrCmd", &Engine::PrintToConsole)
		.addFunction("Selo", Game::GetSelectedObject);

	// variables

	getGlobalNamespace(L)
		.beginNamespace("Mouse")
		.addVariable("RightClick", &Engine::Mouse::RightClick)
		.addVariable("LeftClick", &Engine::Mouse::LeftClick)
		.addFunction("GetLeftClickId", &Picking::Obj::GetLeftClickId)
		.addFunction("GetRightClickId", &Picking::Obj::GetRightClickId)
		.endNamespace();

	// types 

	getGlobalNamespace(L)
		.beginClass<vector<string>>("vector_string")
		.endClass();

	getGlobalNamespace(L)
		.beginClass<vec3>("vec3")
		.endClass();

	// functions

	getGlobalNamespace(L)
		.beginNamespace("Engine")
		.addFunction("GameClose", &Engine::GameClose)
		.addFunction("PrintToConsole", &Engine::PrintToConsole)
		.addFunction("SetEnvironment", &Engine::SetEnvironment)
		.addFunction("GetListOfFolders", Engine::GetListOfFolders)
		.addVariable("WindowWidth", &Engine::myWindow::Width)
		.addVariable("WindowHeight", &Engine::myWindow::Height)
		.endNamespace();

	getGlobalNamespace(L)
		.beginNamespace("Editor")
		.addFunction("Close", &Game::Editor::Close)
		.addFunction("ToggleMenu", &EditorMenuBar::ToggleEditorMenu)
		.addFunction("OpenWindow", &EditorWindows::OpenWindow)
		.addFunction("CloseWindow", &EditorWindows::CloseWindow)
		.addFunction("ToggleWindow", &EditorWindows::ToggleWindow)
		.addFunction("ToggleGrid", &Game::Editor::ToggleGrid)
		.addFunction("GetObjectTreeList1", Game::Editor::GetEditorObjectTreeList1)
		.addFunction("GetObjectTreeList2", Game::Editor::GetEditorObjectTreeList2)
		.addFunction("GetObjectTreeList3", Game::Editor::GetEditorObjectTreeList3)
		.addFunction("GetTerrainTreeList1", Game::Editor::GetEditorTerrainTreeList1)
		.addFunction("GetTerrainTreeList2", Game::Editor::GetEditorTerrainTreeList2)
		.addFunction("InsertObject", Game::Editor::InsertingObject)
		.addFunction("ChangeTerrain", Game::Editor::ChangeTerrainType)
		.endNamespace();

	getGlobalNamespace(L)
		.beginNamespace("Menu")
		.addFunction("OpenMenuPage", &Menu::OpenMenuPage)
		.endNamespace();
	
	getGlobalNamespace(L)
		.beginClass<GObject>("Object")
		.addFunction("GetClassName", &GObject::GetClassName)
		.addFunction("GetIDName", &GObject::GetIDName)
		.addFunction("SetIDName", &GObject::SetIDName)
		.addFunction("GetDisplayedName", &GObject::GetDisplayedName)
		.addFunction("SetDisplayedName", &GObject::SetDisplayedName)
		.endClass();

	getGlobalNamespace(L)
		.beginNamespace("Game")
		.addFunction("IsObjectSelected", &Game::IsGameObjectSelected)
		.addFunction("IsObjectNotNull", &Game::IsGameObjectNotNull)
		.addFunction("CreateObject", &Game::CreateObject)
		.endNamespace();

	getGlobalNamespace(L)
		.beginClass<gui::TextList>("TextList")
		.addFunction("GetSelectedOption", &gui::TextList::GetSelectedOption)
		.endClass();	

	getGlobalNamespace(L)
		.beginClass<gui::TextInput>("TextInput")
		.addFunction("GetText", &gui::TextInput::GetText)
		.endClass();

	getGlobalNamespace(L)
		.beginClass<gui::Iframe>("Iframe")
		.addStaticFunction("GetIframeById", &gui::Iframe::GetIframeById)
		.addFunction("GetTextListById", &gui::Iframe::GetTextListById)
		.addFunction("GetTextInputById", &gui::Iframe::GetTextInputById)
		.addFunction("UpdateTextListById", &gui::Iframe::UpdateTextListById)
		.addFunction("GetStringBySimpleTextId", &gui::Iframe::GetStringBySimpleTextId)
		.addFunction("UpdateStringBySimpleTextId", &gui::Iframe::UpdateStringBySimpleTextId)
		.addFunction("UpdateTextInputPlaceholder", &gui::Iframe::UpdateTextInputPlaceholder)
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

void Hector::ExecuteBooleanMethod(string cmd, bool *boolean)
{
	if (cmd != "") {

		lua_getglobal(L, cmd.c_str());

		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "ERROR ON CALLING FUNCTION: " << lua_tostring(L, -1) << std::endl;
			(*boolean) = false;
		}

		if (!lua_isboolean(L, -1)) {
			std::cout << "ERROR ON X RETURNING TYPE" << std::endl;
			(*boolean) = false;
		}

		else {
			(*boolean) = (bool)lua_toboolean(L, -1);
			lua_pop(L, 1);
		}
	}
}

void Hector::ExecuteStringMethod(string cmd, string *_string)
{
	if (cmd != "") {

		lua_getglobal(L, cmd.c_str());

		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "ERROR ON CALLING FUNCTION: " << lua_tostring(L, -1) << std::endl;
		}

		if (!lua_isstring(L, -1)) {
			std::cout << "ERROR ON RETURNING TYPE" << std::endl;
		}

		else {
			(*_string) = string(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
}

void Hector::ExecuteIntegerMethod(string cmd, int *integer)
{
	if (cmd != "") {

		lua_getglobal(L, cmd.c_str());

		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "ERROR ON CALLING FUNCTION: " << lua_tostring(L, -1) << std::endl;
		}

		if (!lua_isnumber(L, -1)) {
			std::cout << "ERROR ON RETURNING TYPE" << std::endl;
		}

		else {
			(*integer) = int(lua_tonumber(L, -1));
			lua_pop(L, 1);
		}
	}
}

void Hector::ExecuteFloatMethod(string cmd, float *_float)
{
	if (cmd != "") {

		lua_getglobal(L, cmd.c_str());

		if (lua_pcall(L, 0, 1, 0) != 0) {
			std::cout << "ERROR ON CALLING FUNCTION: " << lua_tostring(L, -1) << std::endl;
		}

		if (!lua_isnumber(L, -1)) {
			std::cout << "ERROR ON RETURNING TYPE" << std::endl;
		}

		else {
			(*_float) = (float)lua_tonumber(L, -1);
			lua_pop(L, 1);
		}
	}
}

void Hector::GetIntegerVariable(string name, int * integer)
{
	lua_getglobal(L, name.c_str());
	if (lua_isnil(L, -1)) {
		std::cout << "VARIABLE \"" << name << "\" IS UNDEFINED" << std::endl;
		return;
	}

	(*integer) = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);
}

void Hector::Console::Create()
{
	TEXTINPUT_ID = MAX_NUMBER_OF_TEXT_INPUTS - 1;
	iframe = gui::Iframe("console");
	iframe.Create(MAX_NUMBER_OF_IFRAMES - 1, 30, 30, (int)Engine::myWindow::Width - 60, 30);
	iframe.AddTextInput(TEXTINPUT_ID, 5, 5, (int)Engine::myWindow::Width - 60);
	isOpened = false;
}

void Hector::Console::Render(bool picking)
{
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_F1) && picking == false) {
		isOpened = !isOpened;
		isOpened ? EditorWindows::Hide() : EditorWindows::Show();
		isOpened ? EditorMenuBar::Hide() : EditorMenuBar::Show();
	}

	if (isOpened == false) return;

	if (picking)
	{
		iframe.Render(true);
		return;
	}
	
	iframe.Render();
	string cmd = iframe.GetTextInputById(TEXTINPUT_ID)->GetText();
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ENTER)) {
		if (cmd.size() > 0) {
			ExecuteCommand(cmd);
			iframe.GetTextInputById(TEXTINPUT_ID)->Reset();
		}
	}
	
}


void Hector::CreateConsole()
{
	C.Create();
}

void Hector::RenderConsole(bool picking)
{
	if (Engine::Mouse::LeftClick)
	{
		Picking::UI::ResetClickIds();
		C.Render(true);
		Picking::UI::UpdateClickIds();
	}
	C.Render(false);
}