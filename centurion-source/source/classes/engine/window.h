#pragma once

#include <gui>

using namespace std;
using namespace glm;

namespace engine {

	class myWindow
	{
	public:
		static bool ShouldClose;
		static float HeightZoomed;
		static float WidthZoomed;
		static float Height;
		static float Width;
		static float Ratio;
		static float BottomBarHeight;
		static float TopBarHeight;

		static myWindow GetInstance();
		static void DeleteInstance();

		GLFWwindow* GetGlfwWindow() { return glfwWindow; }
		void clear_buffers();
		void create_callbacks();
		int get_mouse_x() { return (int)lastX; };
		int get_mouse_y() { return (int)lastY; };
		bool* getKeys() { return keys; }
		~myWindow();
	private:
		myWindow();
		void init();

		static myWindow* window;
		GLFWwindow *glfwWindow;
		GLint bufferWidth, bufferHeight;
		bool keys[1024];
		static GLfloat lastX, lastY;

		static void handle_mouse(GLFWwindow* window, double xPos, double yPos);
		static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void character_callback(GLFWwindow* window, unsigned int codepoint);
	};
};