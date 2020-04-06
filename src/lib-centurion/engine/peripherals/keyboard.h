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
	
	};
};

#endif // !KEYBOARD_H