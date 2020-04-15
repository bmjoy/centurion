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

		void Engine::Keyboard::SetKeyStatus(unsigned int key_code, int action)
		{
			keyCode[key_code] = action;
		}

		bool Engine::Keyboard::IsKeyReleased(unsigned int key_code)
		{
			return keyCode[key_code] == 0;
		}

		bool Engine::Keyboard::IsKeyPressed(unsigned int key_code)
		{
			return keyCode[key_code] == 1;
		}

		bool Engine::Keyboard::IsKeyHold(unsigned int key_code)
		{
			return keyCode[key_code] == 2;
		}

		bool Engine::Keyboard::IsKeyNotReleased(unsigned int key_code)
		{
			return keyCode[key_code] != 0;
		}

		int Engine::Keyboard::GetCharCodepointPressed(void)
		{
			return charCodepointPressed;
		}

		void Engine::Keyboard::SetCharCodepointPressed(unsigned int codepoint)
		{
			if (codepoint > 0) {
				charCodepointPressed = codepoint;
			}
			else {
				charCodepointPressed = -1;
			}
		}

		void Engine::Keyboard::ResetKeys(void)
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

		unsigned int Keyboard::KEY_SPACE = GLFW_KEY_SPACE;
		unsigned int Keyboard::KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
		unsigned int Keyboard::KEY_COMMA = GLFW_KEY_COMMA;
		unsigned int Keyboard::KEY_MINUS = GLFW_KEY_MINUS;
		unsigned int Keyboard::KEY_PERIOD = GLFW_KEY_PERIOD;
		unsigned int Keyboard::KEY_SLASH = GLFW_KEY_SLASH;
		unsigned int Keyboard::KEY_0 = GLFW_KEY_0;
		unsigned int Keyboard::KEY_1 = GLFW_KEY_1;
		unsigned int Keyboard::KEY_2 = GLFW_KEY_2;
		unsigned int Keyboard::KEY_3 = GLFW_KEY_3;
		unsigned int Keyboard::KEY_4 = GLFW_KEY_4;
		unsigned int Keyboard::KEY_5 = GLFW_KEY_5;
		unsigned int Keyboard::KEY_6 = GLFW_KEY_6;
		unsigned int Keyboard::KEY_7 = GLFW_KEY_7;
		unsigned int Keyboard::KEY_8 = GLFW_KEY_8;
		unsigned int Keyboard::KEY_9 = GLFW_KEY_9;
		unsigned int Keyboard::KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
		unsigned int Keyboard::KEY_EQUAL = GLFW_KEY_EQUAL;
		unsigned int Keyboard::KEY_A = GLFW_KEY_A;
		unsigned int Keyboard::KEY_B = GLFW_KEY_B;
		unsigned int Keyboard::KEY_C = GLFW_KEY_C;
		unsigned int Keyboard::KEY_D = GLFW_KEY_D;
		unsigned int Keyboard::KEY_E = GLFW_KEY_E;
		unsigned int Keyboard::KEY_F = GLFW_KEY_F;
		unsigned int Keyboard::KEY_G = GLFW_KEY_G;
		unsigned int Keyboard::KEY_H = GLFW_KEY_H;
		unsigned int Keyboard::KEY_I = GLFW_KEY_I;
		unsigned int Keyboard::KEY_J = GLFW_KEY_J;
		unsigned int Keyboard::KEY_K = GLFW_KEY_K;
		unsigned int Keyboard::KEY_L = GLFW_KEY_L;
		unsigned int Keyboard::KEY_M = GLFW_KEY_M;
		unsigned int Keyboard::KEY_N = GLFW_KEY_N;
		unsigned int Keyboard::KEY_O = GLFW_KEY_O;
		unsigned int Keyboard::KEY_P = GLFW_KEY_P;
		unsigned int Keyboard::KEY_Q = GLFW_KEY_Q;
		unsigned int Keyboard::KEY_R = GLFW_KEY_R;
		unsigned int Keyboard::KEY_S = GLFW_KEY_S;
		unsigned int Keyboard::KEY_T = GLFW_KEY_T;
		unsigned int Keyboard::KEY_U = GLFW_KEY_U;
		unsigned int Keyboard::KEY_V = GLFW_KEY_V;
		unsigned int Keyboard::KEY_W = GLFW_KEY_W;
		unsigned int Keyboard::KEY_X = GLFW_KEY_X;
		unsigned int Keyboard::KEY_Y = GLFW_KEY_Y;
		unsigned int Keyboard::KEY_Z = GLFW_KEY_Z;
		unsigned int Keyboard::KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
		unsigned int Keyboard::KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
		unsigned int Keyboard::KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
		unsigned int Keyboard::KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;
		unsigned int Keyboard::KEY_WORLD_1 = GLFW_KEY_WORLD_1;
		unsigned int Keyboard::KEY_WORLD_2 = GLFW_KEY_WORLD_2;
		unsigned int Keyboard::KEY_ESCAPE = GLFW_KEY_ESCAPE;
		unsigned int Keyboard::KEY_ENTER = GLFW_KEY_ENTER;
		unsigned int Keyboard::KEY_TAB = GLFW_KEY_TAB;
		unsigned int Keyboard::KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
		unsigned int Keyboard::KEY_INSERT = GLFW_KEY_INSERT;
		unsigned int Keyboard::KEY_DELETE = GLFW_KEY_DELETE;
		unsigned int Keyboard::KEY_RIGHT = GLFW_KEY_RIGHT;
		unsigned int Keyboard::KEY_LEFT = GLFW_KEY_LEFT;
		unsigned int Keyboard::KEY_DOWN = GLFW_KEY_DOWN;
		unsigned int Keyboard::KEY_UP = GLFW_KEY_UP;
		unsigned int Keyboard::KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
		unsigned int Keyboard::KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
		unsigned int Keyboard::KEY_HOME = GLFW_KEY_HOME;
		unsigned int Keyboard::KEY_END = GLFW_KEY_END;
		unsigned int Keyboard::KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
		unsigned int Keyboard::KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
		unsigned int Keyboard::KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
		unsigned int Keyboard::KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
		unsigned int Keyboard::KEY_PAUSE = GLFW_KEY_PAUSE;
		unsigned int Keyboard::KEY_F1 = GLFW_KEY_F1;
		unsigned int Keyboard::KEY_F2 = GLFW_KEY_F2;
		unsigned int Keyboard::KEY_F3 = GLFW_KEY_F3;
		unsigned int Keyboard::KEY_F4 = GLFW_KEY_F4;
		unsigned int Keyboard::KEY_F5 = GLFW_KEY_F5;
		unsigned int Keyboard::KEY_F6 = GLFW_KEY_F6;
		unsigned int Keyboard::KEY_F7 = GLFW_KEY_F7;
		unsigned int Keyboard::KEY_F8 = GLFW_KEY_F8;
		unsigned int Keyboard::KEY_F9 = GLFW_KEY_F9;
		unsigned int Keyboard::KEY_F10 = GLFW_KEY_F10;
		unsigned int Keyboard::KEY_F11 = GLFW_KEY_F11;
		unsigned int Keyboard::KEY_F12 = GLFW_KEY_F12;
		unsigned int Keyboard::KEY_F13 = GLFW_KEY_F13;
		unsigned int Keyboard::KEY_F14 = GLFW_KEY_F14;
		unsigned int Keyboard::KEY_F15 = GLFW_KEY_F15;
		unsigned int Keyboard::KEY_F16 = GLFW_KEY_F16;
		unsigned int Keyboard::KEY_F17 = GLFW_KEY_F17;
		unsigned int Keyboard::KEY_F18 = GLFW_KEY_F18;
		unsigned int Keyboard::KEY_F19 = GLFW_KEY_F19;
		unsigned int Keyboard::KEY_F20 = GLFW_KEY_F20;
		unsigned int Keyboard::KEY_F21 = GLFW_KEY_F21;
		unsigned int Keyboard::KEY_F22 = GLFW_KEY_F22;
		unsigned int Keyboard::KEY_F23 = GLFW_KEY_F23;
		unsigned int Keyboard::KEY_F24 = GLFW_KEY_F24;
		unsigned int Keyboard::KEY_F25 = GLFW_KEY_F25;
		unsigned int Keyboard::KEY_KP_0 = GLFW_KEY_KP_0;
		unsigned int Keyboard::KEY_KP_1 = GLFW_KEY_KP_1;
		unsigned int Keyboard::KEY_KP_2 = GLFW_KEY_KP_2;
		unsigned int Keyboard::KEY_KP_3 = GLFW_KEY_KP_3;
		unsigned int Keyboard::KEY_KP_4 = GLFW_KEY_KP_4;
		unsigned int Keyboard::KEY_KP_5 = GLFW_KEY_KP_5;
		unsigned int Keyboard::KEY_KP_6 = GLFW_KEY_KP_6;
		unsigned int Keyboard::KEY_KP_7 = GLFW_KEY_KP_7;
		unsigned int Keyboard::KEY_KP_8 = GLFW_KEY_KP_8;
		unsigned int Keyboard::KEY_KP_9 = GLFW_KEY_KP_9;
		unsigned int Keyboard::KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
		unsigned int Keyboard::KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
		unsigned int Keyboard::KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
		unsigned int Keyboard::KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
		unsigned int Keyboard::KEY_KP_ADD = GLFW_KEY_KP_ADD;
		unsigned int Keyboard::KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
		unsigned int Keyboard::KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
		unsigned int Keyboard::KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
		unsigned int Keyboard::KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
		unsigned int Keyboard::KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
		unsigned int Keyboard::KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
		unsigned int Keyboard::KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
		unsigned int Keyboard::KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
		unsigned int Keyboard::KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
		unsigned int Keyboard::KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
		unsigned int Keyboard::KEY_MENU = GLFW_KEY_MENU;
	};
};

