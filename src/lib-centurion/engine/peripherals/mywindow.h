#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <ui.h>

struct GLFWwindow;

namespace Engine {

	class myWindow
	{
	public:
		static float HeightZoomed;
		static float WidthZoomed;
		static float Height;
		static float Width;
		static float Ratio;
		static float BottomBarHeight;
		static float TopBarHeight;

		static myWindow GetInstance(void);
		static void DeleteInstance(void);
		static void TakeScreenshot(void);


		GLFWwindow* GetGlfwWindow() { return glfwWindow; }
		void Create();
		void ClearBuffers();
		void ClearAndSwapBuffers(void);
		void SwapBuffers(void);
		void create_callbacks();
		bool ShouldClose(void) { return shouldClose; }
		void SetShouldClose(void) { shouldClose = true; }
		bool* getKeys() { return keys; }
		~myWindow();
	private:
		myWindow();
		GLFWwindow *glfwWindow;
		GLint bufferWidth, bufferHeight;
		bool keys[1024];
		bool shouldClose;

		static void handle_mouse(GLFWwindow* window, double xPos, double yPos);
		static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void character_callback(GLFWwindow* window, unsigned int codepoint);
		static GLfloat lastX, lastY;
		static myWindow* window;
	};
};

#endif // !MYWINDOW_H