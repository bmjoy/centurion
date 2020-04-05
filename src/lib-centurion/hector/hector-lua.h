/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef HECTOR_LUA_H
#define HECTOR_LUA_H

#include <ui.h>

struct lua_State;

/// <summary>
/// This class handles a complete LUA script system.
/// </summary>
class Hector {
public:

	/// <summary>
	/// This class instances a useful console that can be used to execute specified commands anytime.
	/// </summary>
	class Console {
	public:
		bool IsOpened() { return isOpened; }
		void Create();
		void Render(bool picking);
	private:
		gui::Iframe iframe;
		unsigned int TEXTINPUT_ID;
		bool isOpened;
	};

	/// <summary>
	/// This boolean function checks if the command console is opened or not.
	/// </summary>
	static bool ConsoleIsActive() { return C.IsOpened(); }

	/// <summary>
	/// This function will creates an instance of the command console.
	/// </summary>
	static void CreateConsole();

	/// <summary>
	/// This function handles the command console rendering instructions.
	/// </summary>
	/// <param name="picking">Boolean: true = picking is active; false = normal rendering</param>
	static void RenderConsole(bool picking = false);

	/// <summary>
	/// This function initializes main HECTOR-LUA variables and methods.
	/// </summary>
	static void Initialize();

	/// <summary>
	/// This function tries to execute a specified command by using the console.
	/// </summary>
	/// <param name="cmd">Command that should be executed. Supports strings.</param>
	static void ExecuteCommand(std::string cmd);

	/// <summary>
	/// This function tries to execute a specified boolean method by using the console.
	/// </summary>
	/// <param name="cmd">Command that should be executed. Supports strings.</param>
	/// <param name="*boolean">Default value that should be returned. It supports only referenced booleans.</param>
	static void ExecuteBooleanMethod(std::string cmd, bool *boolean);

	/// <summary>
	/// This function tries to execute a specified string method by using the console.
	/// </summary>
	/// <param name="cmd">Command that should be executed. Supports strings.</param>
	/// <param name="*_string">Default value that should be used. It supports only referenced strings.</param>
	static void ExecuteStringMethod(std::string cmd, std::string *_string);

	/// <summary>
	/// This function tries to execute a specified integer method by using the console.
	/// </summary>
	/// <param name="cmd">Command that should be executed. Supports strings.</param>
	/// <param name="*integer">Default value that should be used. It supports only referenced integers.</param>
	static void ExecuteIntegerMethod(std::string cmd, int *integer);

	/// <summary>
	/// This function tries to execute a specified floating method by using the console.
	/// </summary>
	/// <param name="cmd">Command that should be executed. Supports strings.</param>
	/// <param name="*_float">Default value that should be used. It supports only referenced floats.</param>
	static void ExecuteFloatMethod(std::string cmd, float *_float);

	/// <summary>
	/// This function is used to return the value of a specified integer variable.
	/// </summary>
	/// <param name="name">Name of the variable that should be used. Supports strings.</param>
	/// <param name="*integer">Default value. It supports only referenced integers.</param>
	static void GetIntegerVariable(std::string name, int *integer);

private:
	static lua_State* L;
	static Console C;
};

#endif