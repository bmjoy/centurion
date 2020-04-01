#include <engine.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

// static variables
int Engine::Keyboard::keyCode[348] = { 0 };
int Engine::Keyboard::charCodepointPressed = -1;

void Engine::Keyboard::SetCharCodepointPressed(int codepoint)
{
	if (codepoint > 0) {
		charCodepointPressed = codepoint;
	}
	else {
		charCodepointPressed = -1;
	}
}

void Engine::Keyboard::ResetKeys()
{
	for (int key = 0; key < GLFW_KEY_LAST; key++) {
		if (key == GLFW_KEY_LEFT_CONTROL) continue;
		if (key == GLFW_KEY_RIGHT_CONTROL) continue;
		if (key == GLFW_KEY_LEFT_SHIFT) continue;
		if (key == GLFW_KEY_RIGHT_SHIFT) continue;
		if (key == GLFW_KEY_LEFT_ALT) continue;
		if (key == GLFW_KEY_RIGHT_ALT) continue;

		keyCode[key] = GLFW_RELEASE;
	}
}
