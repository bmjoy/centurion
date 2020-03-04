#include <game/strategy.h>
#include "window.h"
#include "mouse.h"

using namespace glb;


namespace engine {

	// Define static variables (default values)
	bool myWindow::ShouldClose = false;
	float myWindow::WidthZoomed = 1366.f;
	float myWindow::HeightZoomed = 768.f;
	float myWindow::Width = 1366.f;
	float myWindow::Height = 768.f;
	float myWindow::Ratio = 1.f;
	float myWindow::BottomBarHeight = 0;
	float myWindow::TopBarHeight = 0;
	GLfloat myWindow::lastX = 0;
	GLfloat myWindow::lastY = 0;
	myWindow* myWindow::window;
	// ------------ End definition

	myWindow::myWindow() {}
	   
	myWindow myWindow::GetInstance() {
		if (window == nullptr) {
			window = new myWindow();
			window->init();
		}
		return (*window);
	}

	void myWindow::init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		if (!Settings::FullScreen) glfwWindow = glfwCreateWindow((int)Width, (int)Height, gameNameSTR.c_str(), nullptr, nullptr); // Windowed
		else glfwWindow = glfwCreateWindow((int)Width, (int)Height, gameNameSTR.c_str(), glfwGetPrimaryMonitor(), nullptr); // full screen
		glfwMakeContextCurrent(glfwWindow);
		create_callbacks(); // handle key + mouse input

		// Initialize GLEW to setup the OpenGL Function pointers
		glewExperimental = GL_TRUE;
		glewInit();
		glfwGetFramebufferSize(glfwWindow, &bufferWidth, &bufferHeight);

		// Define the viewport dimensions
		glViewport(0, 0, bufferWidth, bufferHeight);
		glfwSetWindowUserPointer(glfwWindow, this); // this function is for taking inputs

		// Set OpenGL options
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// hide the mouse cursor in the window
	}

	void myWindow::clear_buffers() {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (glfwWindowShouldClose(glfwWindow)) {
			ShouldClose = true;
		}
	}

	void myWindow::create_callbacks() { // important for taking the keyboard / mouse input 
		glfwSetKeyCallback(glfwWindow, handle_keys);
		glfwSetCursorPosCallback(glfwWindow, handle_mouse);
		glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
		glfwSetScrollCallback(glfwWindow, scroll_callback);
		glfwSetCharCallback(glfwWindow, character_callback);
	}

	void myWindow::character_callback(GLFWwindow* window, unsigned int codepoint) {
		CharCodepointPressed = (int)codepoint;
	}

	void myWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) {
		KeyCode[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	void myWindow::handle_mouse(GLFWwindow* window, double xPos, double yPos) {
		lastX = (GLfloat)xPos;
		double y = fabs(yPos - Height);
		lastY = (GLfloat)y;
	}

	void myWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {
				Mouse::LeftClick = true;
				Mouse::Release = false;
				Mouse::LeftHold = true;
			}
			else if (action == GLFW_RELEASE) {
				Mouse::LeftClick = false;
				Mouse::Release = true;
				Mouse::LeftHold = false;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				Mouse::RightClick = true;
				Mouse::Release = false;
			}
			else if (action == GLFW_RELEASE) {
				Mouse::RightClick = false;
				Mouse::Release = true;
			}
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if (action == GLFW_PRESS) {
				Mouse::MiddleClick = true;
			}
			else if (action == GLFW_RELEASE) {
				Mouse::MiddleClick = false;
			}
		}
	}

	void myWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
		Mouse::ScrollValue = float(yoffset);
		Mouse::ScrollBool = true;
	}

	myWindow::~myWindow(){
	}
}
