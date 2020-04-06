#include <engine.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace Engine
{
	namespace Keyboard 
	{

		// Private variables
		namespace 
		{
			int keyCode[348] = { 0 };
			int charCodepointPressed = -1;

		};

		void SetKeyStatus(unsigned int key_code, int action)
		{
			keyCode[key_code] = action;
		}

		bool IsKeyReleased(unsigned int key_code)
		{
			return keyCode[key_code] == 0;
		}

		bool IsKeyPressed(unsigned int key_code)
		{
			return keyCode[key_code] == 1;
		}

		bool IsKeyHold(unsigned int key_code)
		{
			return keyCode[key_code] == 2;
		}

		bool IsKeyNotReleased(unsigned int key_code)
		{
			return keyCode[key_code] != 0;
		}

		int GetCharCodepointPressed(void)
		{
			return charCodepointPressed;
		}

		void SetCharCodepointPressed(unsigned int codepoint)
		{
			if (codepoint > 0) {
				charCodepointPressed = codepoint;
			}
			else {
				charCodepointPressed = -1;
			}
		}

		void ResetKeys(void)
		{
			for (int key = 0; key < GLFW_KEY_LAST; key++) {
				if (key == GLFW_KEY_LEFT_CONTROL) continue;
				if (key == GLFW_KEY_RIGHT_CONTROL) continue;
				if (key == GLFW_KEY_LEFT_SHIFT) continue;
				if (key == GLFW_KEY_RIGHT_SHIFT) continue;
				if (key == GLFW_KEY_LEFT_ALT) continue;
				if (key == GLFW_KEY_RIGHT_ALT) continue;
				if (key == GLFW_KEY_RIGHT) continue;
				if (key == GLFW_KEY_LEFT) continue;
				if (key == GLFW_KEY_UP) continue;
				if (key == GLFW_KEY_DOWN) continue;

				keyCode[key] = GLFW_RELEASE;
			}
		}

	};
};

