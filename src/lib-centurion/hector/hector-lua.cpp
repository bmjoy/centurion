#include "hector-lua.h"
#include "hector-misc.h"

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
#include <classes/building.h>
#include <classes/settlement.h>

#include <ui.h>
#include <file_manager.h>

#include <GLFW/glfw3.h>

using namespace luabridge;
using namespace std;
using namespace glm;

namespace Hector
{
	// private variables 
	namespace
	{
		lua_State* L;
		Console C;
	};

	void AddGlfwKeysToNamespace(void);

	void Hector::Initialize()
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		// no namespace functions

		getGlobalNamespace(L)
			.addFunction("PrCmd", &Hector::PrCmd)
			.addFunction("ToggleWireframe", &Hector::ToggleWireframe)
			.addFunction("ToggleGrid", &Hector::ToggleGrid)
			.addFunction("Selo", Hector::Selo)
			.addFunction("IsQuestionWindowActive", &Hector::IsQuestionWindowActive)
			.addFunction("GetQuestionWindowText", &Hector::GetQuestionWindowText)
			.addFunction("ResetQuestionWindow", &Hector::ResetQuestionWindow)
			.addFunction("ExecuteYesCmdOfQuestionWindow", &Hector::ExecuteYesCmdOfQuestionWindow)
			.addFunction("ExecuteNoCmdOfQuestionWindow", &Hector::ExecuteNoCmdOfQuestionWindow)
			.addFunction("IsInfoBoxWindowActive", &Hector::IsInfoBoxWindowActive)
			.addFunction("GetInfoBoxWindowText", &Hector::GetInfoBoxWindowText)
			.addFunction("ResetInfoBoxWindow", &Hector::ResetInfoBoxWindow)
			;

		// variables

		AddGlfwKeysToNamespace();

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
			.addFunction("SetEnvironment", &Engine::SetEnvironment)
			.addFunction("GetListOfFolders", Engine::GetListOfFolders)
			.addVariable("WindowWidth", &Engine::myWindow::Width)
			.addVariable("WindowHeight", &Engine::myWindow::Height)
			.endNamespace();

		getGlobalNamespace(L)
			.beginNamespace("Editor")
			.addFunction("Close", &Game::Editor::Close)
			.addFunction("AskForClosing", &Game::Editor::AskForClosing)
			.addFunction("GetObjectTreeList1", Game::Editor::GetEditorObjectTreeList1)
			.addFunction("GetObjectTreeList2", Game::Editor::GetEditorObjectTreeList2)
			.addFunction("GetObjectTreeList3", Game::Editor::GetEditorObjectTreeList3)
			.addFunction("GetTerrainTreeList1", Game::Editor::GetEditorTerrainTreeList1)
			.addFunction("GetTerrainTreeList2", Game::Editor::GetEditorTerrainTreeList2)
			.addFunction("InsertObject", &Game::Editor::InsertingObject)
			.addFunction("ChangeTerrain", &Game::Editor::ChangeTerrainType)
			.addFunction("GenerateRandomMap", &Game::Editor::GenerateRandomMap)
			.addFunction("ToggleMenu", &EditorMenuBar::ToggleEditorMenu)
			.addFunction("OpenWindow", &EditorWindows::OpenWindow)
			.addFunction("CloseWindow", &EditorWindows::CloseWindow)
			.addFunction("ToggleWindow", &EditorWindows::ToggleWindow)
			.addFunction("SaveScenario", &Game::Map::SaveScenario)
			.addFunction("LoadScenario", &Game::Map::LoadScenario)
			.addFunction("DeleteScenario", &Game::Map::DeleteScenario)
			.addFunction("MarkAsEdited", &Game::Map::MarkAsEdited)
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
			.addFunction("GetID", &GObject::GetPickingID)
			.addFunction("GetDisplayedName", &GObject::GetDisplayedName)
			.addFunction("SetDisplayedName", &GObject::SetDisplayedName)
			.addFunction("AsBuilding", &GObject::AsBuilding)
			.addStaticFunction("Remove", &GObject::RemoveGameObject)
			.endClass();

		getGlobalNamespace(L)
			.beginClass<Building>("Building")
			.addFunction("GetSettlement", &Building::GetSettlement)
			.endClass();

		getGlobalNamespace(L)
			.beginClass<Settlement>("Settlement")
			.addFunction("GetSettlementName", &Settlement::GetSettlementName)
			.addFunction("SetSettlementName", &Settlement::SetSettlementName)
			.addFunction("SettlementDestroy", &Settlement::SettlementDestroy)
			.endClass();

		getGlobalNamespace(L)
			.beginNamespace("Game")
			.addFunction("IsObjectSelected", &Game::IsGameObjectSelected)
			.addFunction("IsObjectNotNull", &Game::IsGameObjectNotNull)
			.addFunction("ResetSelectedObject", &Game::ResetSelectedObject)
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
				Hector::PrCmd(errormsg);
			}
		}
	}

	void Hector::ExecuteBooleanMethod(string cmd, bool* boolean)
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

	void Hector::ExecuteStringMethod(string cmd, string* _string)
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

	void Hector::ExecuteIntegerMethod(string cmd, int* integer)
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

	void Hector::ExecuteFloatMethod(string cmd, float* _float)
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

	void Hector::GetIntegerVariable(string name, int* integer)
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
		history = std::deque<std::string>();
		history_size = 10;
		history_selected = 0;
	}

	void Hector::Console::Render(bool picking)
	{
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_F1) && picking == false) {
			if (isOpened == false)
			{
				isOpened = true;
				iframe.Open();
				iframe.GetTextInputById(TEXTINPUT_ID)->EnableTextInput();
				EditorWindows::Hide();
				EditorMenuBar::Hide();
			}
			else {
				isOpened = false;
				iframe.Close();
				EditorWindows::Show();
				EditorMenuBar::Show();
			}
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
			if (cmd.empty() == false) {
				ExecuteCommand(cmd);
				iframe.GetTextInputById(TEXTINPUT_ID)->Reset();

				history_selected = 0;
				history.push_front(cmd);

				if (history.size() > history_size)
				{
					history.pop_back();
				}
			}
		}
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_UP))
		{
			if (history.size() == 0) return;
			if (history_selected > history.size() - 1) return;
			iframe.GetTextInputById(TEXTINPUT_ID)->UpdatePlaceholder(history[history_selected]);
			history_selected++;

			Engine::Keyboard::SetKeyStatus(GLFW_KEY_UP, 0);
		}
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_DOWN))
		{
			history_selected = 0;
			iframe.GetTextInputById(TEXTINPUT_ID)->UpdatePlaceholder("");
			Engine::Keyboard::SetKeyStatus(GLFW_KEY_DOWN, 0);
		}

	}

	bool Hector::ConsoleIsActive()
	{
		return C.IsOpened();
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

	void Hector::AddGlfwKeysToNamespace(void)
	{
		getGlobalNamespace(L)
			.beginNamespace("Keyboard")
			.addFunction("IsKeyPressed", &Engine::Keyboard::IsKeyPressed)
			.addVariable("KEY_SPACE", &Engine::Keyboard::KEY_SPACE)
			.addVariable("KEY_APOSTROPHE", &Engine::Keyboard::KEY_APOSTROPHE)
			.addVariable("KEY_COMMA", &Engine::Keyboard::KEY_COMMA)
			.addVariable("KEY_MINUS", &Engine::Keyboard::KEY_MINUS)
			.addVariable("KEY_PERIOD", &Engine::Keyboard::KEY_PERIOD)
			.addVariable("KEY_SLASH", &Engine::Keyboard::KEY_SLASH)
			.addVariable("KEY_0", &Engine::Keyboard::KEY_0)
			.addVariable("KEY_1", &Engine::Keyboard::KEY_1)
			.addVariable("KEY_2", &Engine::Keyboard::KEY_2)
			.addVariable("KEY_3", &Engine::Keyboard::KEY_3)
			.addVariable("KEY_4", &Engine::Keyboard::KEY_4)
			.addVariable("KEY_5", &Engine::Keyboard::KEY_5)
			.addVariable("KEY_6", &Engine::Keyboard::KEY_6)
			.addVariable("KEY_7", &Engine::Keyboard::KEY_7)
			.addVariable("KEY_8", &Engine::Keyboard::KEY_8)
			.addVariable("KEY_9", &Engine::Keyboard::KEY_9)
			.addVariable("KEY_SEMICOLON", &Engine::Keyboard::KEY_SEMICOLON)
			.addVariable("KEY_EQUAL", &Engine::Keyboard::KEY_EQUAL)
			.addVariable("KEY_A", &Engine::Keyboard::KEY_A)
			.addVariable("KEY_B", &Engine::Keyboard::KEY_B)
			.addVariable("KEY_C", &Engine::Keyboard::KEY_C)
			.addVariable("KEY_D", &Engine::Keyboard::KEY_D)
			.addVariable("KEY_E", &Engine::Keyboard::KEY_E)
			.addVariable("KEY_F", &Engine::Keyboard::KEY_F)
			.addVariable("KEY_G", &Engine::Keyboard::KEY_G)
			.addVariable("KEY_H", &Engine::Keyboard::KEY_H)
			.addVariable("KEY_I", &Engine::Keyboard::KEY_I)
			.addVariable("KEY_J", &Engine::Keyboard::KEY_J)
			.addVariable("KEY_K", &Engine::Keyboard::KEY_K)
			.addVariable("KEY_L", &Engine::Keyboard::KEY_L)
			.addVariable("KEY_M", &Engine::Keyboard::KEY_M)
			.addVariable("KEY_N", &Engine::Keyboard::KEY_N)
			.addVariable("KEY_O", &Engine::Keyboard::KEY_O)
			.addVariable("KEY_P", &Engine::Keyboard::KEY_P)
			.addVariable("KEY_Q", &Engine::Keyboard::KEY_Q)
			.addVariable("KEY_R", &Engine::Keyboard::KEY_R)
			.addVariable("KEY_S", &Engine::Keyboard::KEY_S)
			.addVariable("KEY_T", &Engine::Keyboard::KEY_T)
			.addVariable("KEY_U", &Engine::Keyboard::KEY_U)
			.addVariable("KEY_V", &Engine::Keyboard::KEY_V)
			.addVariable("KEY_W", &Engine::Keyboard::KEY_W)
			.addVariable("KEY_X", &Engine::Keyboard::KEY_X)
			.addVariable("KEY_Y", &Engine::Keyboard::KEY_Y)
			.addVariable("KEY_Z", &Engine::Keyboard::KEY_Z)
			.addVariable("KEY_LEFT_BRACKET", &Engine::Keyboard::KEY_LEFT_BRACKET)
			.addVariable("KEY_BACKSLASH", &Engine::Keyboard::KEY_BACKSLASH)
			.addVariable("KEY_RIGHT_BRACKET", &Engine::Keyboard::KEY_RIGHT_BRACKET)
			.addVariable("KEY_GRAVE_ACCENT", &Engine::Keyboard::KEY_GRAVE_ACCENT)
			.addVariable("KEY_WORLD_1", &Engine::Keyboard::KEY_WORLD_1)
			.addVariable("KEY_WORLD_2", &Engine::Keyboard::KEY_WORLD_2)
			.addVariable("KEY_ESCAPE", &Engine::Keyboard::KEY_ESCAPE)
			.addVariable("KEY_ENTER", &Engine::Keyboard::KEY_ENTER)
			.addVariable("KEY_TAB", &Engine::Keyboard::KEY_TAB)
			.addVariable("KEY_BACKSPACE", &Engine::Keyboard::KEY_BACKSPACE)
			.addVariable("KEY_INSERT", &Engine::Keyboard::KEY_INSERT)
			.addVariable("KEY_DELETE", &Engine::Keyboard::KEY_DELETE)
			.addVariable("KEY_RIGHT", &Engine::Keyboard::KEY_RIGHT)
			.addVariable("KEY_LEFT", &Engine::Keyboard::KEY_LEFT)
			.addVariable("KEY_DOWN", &Engine::Keyboard::KEY_DOWN)
			.addVariable("KEY_UP", &Engine::Keyboard::KEY_UP)
			.addVariable("KEY_PAGE_UP", &Engine::Keyboard::KEY_PAGE_UP)
			.addVariable("KEY_PAGE_DOWN", &Engine::Keyboard::KEY_PAGE_DOWN)
			.addVariable("KEY_HOME", &Engine::Keyboard::KEY_HOME)
			.addVariable("KEY_END", &Engine::Keyboard::KEY_END)
			.addVariable("KEY_CAPS_LOCK", &Engine::Keyboard::KEY_CAPS_LOCK)
			.addVariable("KEY_SCROLL_LOCK", &Engine::Keyboard::KEY_SCROLL_LOCK)
			.addVariable("KEY_NUM_LOCK", &Engine::Keyboard::KEY_NUM_LOCK)
			.addVariable("KEY_PRINT_SCREEN", &Engine::Keyboard::KEY_PRINT_SCREEN)
			.addVariable("KEY_PAUSE", &Engine::Keyboard::KEY_PAUSE)
			.addVariable("KEY_F1", &Engine::Keyboard::KEY_F1)
			.addVariable("KEY_F2", &Engine::Keyboard::KEY_F2)
			.addVariable("KEY_F3", &Engine::Keyboard::KEY_F3)
			.addVariable("KEY_F4", &Engine::Keyboard::KEY_F4)
			.addVariable("KEY_F5", &Engine::Keyboard::KEY_F5)
			.addVariable("KEY_F6", &Engine::Keyboard::KEY_F6)
			.addVariable("KEY_F7", &Engine::Keyboard::KEY_F7)
			.addVariable("KEY_F8", &Engine::Keyboard::KEY_F8)
			.addVariable("KEY_F9", &Engine::Keyboard::KEY_F9)
			.addVariable("KEY_F10", &Engine::Keyboard::KEY_F10)
			.addVariable("KEY_F11", &Engine::Keyboard::KEY_F11)
			.addVariable("KEY_F12", &Engine::Keyboard::KEY_F12)
			.addVariable("KEY_F13", &Engine::Keyboard::KEY_F13)
			.addVariable("KEY_F14", &Engine::Keyboard::KEY_F14)
			.addVariable("KEY_F15", &Engine::Keyboard::KEY_F15)
			.addVariable("KEY_F16", &Engine::Keyboard::KEY_F16)
			.addVariable("KEY_F17", &Engine::Keyboard::KEY_F17)
			.addVariable("KEY_F18", &Engine::Keyboard::KEY_F18)
			.addVariable("KEY_F19", &Engine::Keyboard::KEY_F19)
			.addVariable("KEY_F20", &Engine::Keyboard::KEY_F20)
			.addVariable("KEY_F21", &Engine::Keyboard::KEY_F21)
			.addVariable("KEY_F22", &Engine::Keyboard::KEY_F22)
			.addVariable("KEY_F23", &Engine::Keyboard::KEY_F23)
			.addVariable("KEY_F24", &Engine::Keyboard::KEY_F24)
			.addVariable("KEY_F25", &Engine::Keyboard::KEY_F25)
			.addVariable("KEY_KP_0", &Engine::Keyboard::KEY_KP_0)
			.addVariable("KEY_KP_1", &Engine::Keyboard::KEY_KP_1)
			.addVariable("KEY_KP_2", &Engine::Keyboard::KEY_KP_2)
			.addVariable("KEY_KP_3", &Engine::Keyboard::KEY_KP_3)
			.addVariable("KEY_KP_4", &Engine::Keyboard::KEY_KP_4)
			.addVariable("KEY_KP_5", &Engine::Keyboard::KEY_KP_5)
			.addVariable("KEY_KP_6", &Engine::Keyboard::KEY_KP_6)
			.addVariable("KEY_KP_7", &Engine::Keyboard::KEY_KP_7)
			.addVariable("KEY_KP_8", &Engine::Keyboard::KEY_KP_8)
			.addVariable("KEY_KP_9", &Engine::Keyboard::KEY_KP_9)
			.addVariable("KEY_KP_DECIMAL", &Engine::Keyboard::KEY_KP_DECIMAL)
			.addVariable("KEY_KP_DIVIDE", &Engine::Keyboard::KEY_KP_DIVIDE)
			.addVariable("KEY_KP_MULTIPLY", &Engine::Keyboard::KEY_KP_MULTIPLY)
			.addVariable("KEY_KP_SUBTRACT", &Engine::Keyboard::KEY_KP_SUBTRACT)
			.addVariable("KEY_KP_ADD", &Engine::Keyboard::KEY_KP_ADD)
			.addVariable("KEY_KP_ENTER", &Engine::Keyboard::KEY_KP_ENTER)
			.addVariable("KEY_KP_EQUAL", &Engine::Keyboard::KEY_KP_EQUAL)
			.addVariable("KEY_LEFT_SHIFT", &Engine::Keyboard::KEY_LEFT_SHIFT)
			.addVariable("KEY_LEFT_CONTROL", &Engine::Keyboard::KEY_LEFT_CONTROL)
			.addVariable("KEY_LEFT_ALT", &Engine::Keyboard::KEY_LEFT_ALT)
			.addVariable("KEY_LEFT_SUPER", &Engine::Keyboard::KEY_LEFT_SUPER)
			.addVariable("KEY_RIGHT_SHIFT", &Engine::Keyboard::KEY_RIGHT_SHIFT)
			.addVariable("KEY_RIGHT_CONTROL", &Engine::Keyboard::KEY_RIGHT_CONTROL)
			.addVariable("KEY_RIGHT_ALT", &Engine::Keyboard::KEY_RIGHT_ALT)
			.addVariable("KEY_RIGHT_SUPER", &Engine::Keyboard::KEY_RIGHT_SUPER)
			.addVariable("KEY_MENU", &Engine::Keyboard::KEY_MENU)
			.endNamespace();
	}
};

