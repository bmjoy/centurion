/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ui.h>

namespace Engine {

	/// <summary>
	/// This namespace handles the keyboard.
	/// </summary>
	namespace Keyboard {

		/// <summary>
		/// This function sets the current status of a button.
		/// </summary>
		/// <param name="key_code">Button code. Accepted value is between 0 and 347</param>
		/// <param name="action">State of the button to be set. Accepted values are 0 = released; 1 = pressed; 2 = hold;</param>
		void SetKeyStatus(unsigned int key_code, int action);

		/// <summary>
		/// This function detects whenever a button is being released.
		/// </summary>
		/// <param name="key_code">Button code. Accepted value is between 0 and 347</param>
		/// <returns>This value it's true if the specified button is begin released; it's false otherwise</returns>
		bool IsKeyReleased(unsigned int key_code);

		/// <summary>
		/// This function detects whenever a button is being pressed.
		/// </summary>
		/// <param name="key_code">Button code. Accepted value is between 0 and 347</param>
		/// <returns>This value it's true if the specified button is begin pressed; it's false otherwise</returns>
		bool IsKeyPressed(unsigned int key_code);

		/// <summary>
		/// This function detects whenever a button is being hold.
		/// </summary>
		/// <param name="key_code">Button code. Accepted value is between 0 and 347</param>
		/// <returns>This value it's true if the specified button is begin hold; it's false otherwise</returns>
		bool IsKeyHold(unsigned int key_code);

		/// <summary>
		/// This function detects whenever a button is not being pressed or hold.
		/// </summary>
		/// <param name="key_code">Button code. Accepted value is between 0 and 347</param>
		/// <returns>This value it's true if the specified button is not being pressed or hold; it's false otherwise</returns>
		bool IsKeyNotReleased(unsigned int key_code);

		/// <summary>
		/// This function gets the char codepoint whenever is being pressed
		/// </summary>
		/// <returns>Char codepoint that should be returned, expressed in integer</returns>
		int GetCharCodepointPressed(void);

		/// <summary>
		/// This function sets the char codepoint, for example from a callback
		/// </summary>
		/// <param name="codepoint">Selected char codepoint.It supports only unsigned int</param>
		void SetCharCodepointPressed(unsigned int codepoint);

		/// <summary>
		/// This function resets all the key status.
		/// </summary>
		void ResetKeys(void);

		// List of keys 
		extern unsigned int KEY_SPACE;
		extern unsigned int KEY_APOSTROPHE;
		extern unsigned int KEY_COMMA;
		extern unsigned int KEY_MINUS;
		extern unsigned int KEY_PERIOD;
		extern unsigned int KEY_SLASH;
		extern unsigned int KEY_0;
		extern unsigned int KEY_1;
		extern unsigned int KEY_2;
		extern unsigned int KEY_3;
		extern unsigned int KEY_4;
		extern unsigned int KEY_5;
		extern unsigned int KEY_6;
		extern unsigned int KEY_7;
		extern unsigned int KEY_8;
		extern unsigned int KEY_9;
		extern unsigned int KEY_SEMICOLON;
		extern unsigned int KEY_EQUAL;
		extern unsigned int KEY_A;
		extern unsigned int KEY_B;
		extern unsigned int KEY_C;
		extern unsigned int KEY_D;
		extern unsigned int KEY_E;
		extern unsigned int KEY_F;
		extern unsigned int KEY_G;
		extern unsigned int KEY_H;
		extern unsigned int KEY_I;
		extern unsigned int KEY_J;
		extern unsigned int KEY_K;
		extern unsigned int KEY_L;
		extern unsigned int KEY_M;
		extern unsigned int KEY_N;
		extern unsigned int KEY_O;
		extern unsigned int KEY_P;
		extern unsigned int KEY_Q;
		extern unsigned int KEY_R;
		extern unsigned int KEY_S;
		extern unsigned int KEY_T;
		extern unsigned int KEY_U;
		extern unsigned int KEY_V;
		extern unsigned int KEY_W;
		extern unsigned int KEY_X;
		extern unsigned int KEY_Y;
		extern unsigned int KEY_Z;
		extern unsigned int KEY_LEFT_BRACKET;
		extern unsigned int KEY_BACKSLASH;
		extern unsigned int KEY_RIGHT_BRACKET;
		extern unsigned int KEY_GRAVE_ACCENT;
		extern unsigned int KEY_WORLD_1;
		extern unsigned int KEY_WORLD_2;
		extern unsigned int KEY_ESCAPE;
		extern unsigned int KEY_ENTER;
		extern unsigned int KEY_TAB;
		extern unsigned int KEY_BACKSPACE;
		extern unsigned int KEY_INSERT;
		extern unsigned int KEY_DELETE;
		extern unsigned int KEY_RIGHT;
		extern unsigned int KEY_LEFT;
		extern unsigned int KEY_DOWN;
		extern unsigned int KEY_UP;
		extern unsigned int KEY_PAGE_UP;
		extern unsigned int KEY_PAGE_DOWN;
		extern unsigned int KEY_HOME;
		extern unsigned int KEY_END;
		extern unsigned int KEY_CAPS_LOCK;
		extern unsigned int KEY_SCROLL_LOCK;
		extern unsigned int KEY_NUM_LOCK;
		extern unsigned int KEY_PRINT_SCREEN;
		extern unsigned int KEY_PAUSE;
		extern unsigned int KEY_F1;
		extern unsigned int KEY_F2;
		extern unsigned int KEY_F3;
		extern unsigned int KEY_F4;
		extern unsigned int KEY_F5;
		extern unsigned int KEY_F6;
		extern unsigned int KEY_F7;
		extern unsigned int KEY_F8;
		extern unsigned int KEY_F9;
		extern unsigned int KEY_F10;
		extern unsigned int KEY_F11;
		extern unsigned int KEY_F12;
		extern unsigned int KEY_F13;
		extern unsigned int KEY_F14;
		extern unsigned int KEY_F15;
		extern unsigned int KEY_F16;
		extern unsigned int KEY_F17;
		extern unsigned int KEY_F18;
		extern unsigned int KEY_F19;
		extern unsigned int KEY_F20;
		extern unsigned int KEY_F21;
		extern unsigned int KEY_F22;
		extern unsigned int KEY_F23;
		extern unsigned int KEY_F24;
		extern unsigned int KEY_F25;
		extern unsigned int KEY_KP_0;
		extern unsigned int KEY_KP_1;
		extern unsigned int KEY_KP_2;
		extern unsigned int KEY_KP_3;
		extern unsigned int KEY_KP_4;
		extern unsigned int KEY_KP_5;
		extern unsigned int KEY_KP_6;
		extern unsigned int KEY_KP_7;
		extern unsigned int KEY_KP_8;
		extern unsigned int KEY_KP_9;
		extern unsigned int KEY_KP_DECIMAL;
		extern unsigned int KEY_KP_DIVIDE;
		extern unsigned int KEY_KP_MULTIPLY;
		extern unsigned int KEY_KP_SUBTRACT;
		extern unsigned int KEY_KP_ADD;
		extern unsigned int KEY_KP_ENTER;
		extern unsigned int KEY_KP_EQUAL;
		extern unsigned int KEY_LEFT_SHIFT;
		extern unsigned int KEY_LEFT_CONTROL;
		extern unsigned int KEY_LEFT_ALT;
		extern unsigned int KEY_LEFT_SUPER;
		extern unsigned int KEY_RIGHT_SHIFT;
		extern unsigned int KEY_RIGHT_CONTROL;
		extern unsigned int KEY_RIGHT_ALT;
		extern unsigned int KEY_RIGHT_SUPER;
		extern unsigned int KEY_MENU;
	
	};
};

#endif // !KEYBOARD_H