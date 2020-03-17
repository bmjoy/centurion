#ifndef HECTOR_LUA_H
#define HECTOR_LUA_H

#include <string>

using namespace std;
struct lua_State;

class Hector {
public:
	static void Initialize();
	static void ExecuteCommand(string cmd);
	static bool ExecuteBooleanMethod(string cmd);

private:
	static lua_State* L;
};

#endif