#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ui.h>

namespace Engine {

	namespace Keyboard {

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key_code"></param>
		/// <param name="action"></param>
		void SetKeyStatus(unsigned int key_code, int action);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key_code"></param>
		/// <returns></returns>
		bool IsKeyReleased(unsigned int key_code);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key_code"></param>
		/// <returns></returns>
		bool IsKeyPressed(unsigned int key_code);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key_code"></param>
		/// <returns></returns>
		bool IsKeyHold(unsigned int key_code);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key_code"></param>
		/// <returns></returns>
		bool IsKeyNotReleased(unsigned int key_code);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetCharCodepointPressed(void);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="codepoint"></param>
		void SetCharCodepointPressed(unsigned int codepoint);

		/// <summary>
		/// 
		/// </summary>
		void ResetKeys(void);
	
	};
};

#endif // !KEYBOARD_H