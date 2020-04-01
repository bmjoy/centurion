#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ui.h>

namespace Engine {

	class Keyboard {
	public:
		static void SetKeyStatus(unsigned int key_code, int action) { keyCode[key_code] = action; }
		static bool IsKeyReleased(unsigned int key_code) { return keyCode[key_code] == 0; }
		static bool IsKeyPressed(unsigned int key_code) { return keyCode[key_code] == 1; }
		static bool IsKeyHold(unsigned int key_code) { return keyCode[key_code] == 2; }
		static bool IsKeyNotReleased(unsigned int key_code) { return keyCode[key_code] != 0; }
		static int GetCharCodepointPressed(void) { return charCodepointPressed; }
		static void SetCharCodepointPressed(int codepoint);
		static void ResetKeys();
	private:
		static int keyCode[348];
		static int charCodepointPressed;
	};

};

#endif // !KEYBOARD_H