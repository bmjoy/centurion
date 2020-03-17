#include "hector-lua.h"

#include <LUA/lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <menu/menu.h>
#include <engine.h>

#include "game/interface/editorMenuBar.h"

using namespace luabridge;

lua_State* Hector::L;

void Hector::Initialize()
{
	L = luaL_newstate();
	luaL_openlibs(L);

	getGlobalNamespace(L).addFunction("GameClose", Engine::GameClose);
	getGlobalNamespace(L).addFunction("SetEnvironment", Engine::SetEnvironment);
	getGlobalNamespace(L).addFunction("OpenMenuPage", Menu::OpenMenuPage);
	getGlobalNamespace(L).addFunction("ToggleEditorMenu", EditorMenuBar::ToggleEditorMenu);
}

void Hector::ExecuteCommand(string cmd)
{
	if (cmd != "") {
		luaL_dostring(L, cmd.c_str());
	}
}
