#include <engine.h>

#include <Settings.h>
#include <Logger.h>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

// Define static variables (default values)
bool Engine::myWindow::ShouldClose = false;
float Engine::myWindow::WidthZoomed = 1366.f;
float Engine::myWindow::HeightZoomed = 768.f;
float Engine::myWindow::Width = 1366.f;
float Engine::myWindow::Height = 768.f;
float Engine::myWindow::Ratio = 1.f;
float Engine::myWindow::BottomBarHeight = 0;
float Engine::myWindow::TopBarHeight = 0;
GLfloat Engine::myWindow::lastX = 0;
GLfloat Engine::myWindow::lastY = 0;
Engine::myWindow* Engine::myWindow::window;
// ------------ End definition

Engine::myWindow::myWindow() {}

Engine::myWindow Engine::myWindow::GetInstance(void) {
	if (window == nullptr) {
		window = new myWindow();
		window->init();
	}
	return (*window);
}

void Engine::myWindow::DeleteInstance(void) {
	if (window != nullptr) {
		delete window;
	}
}

void Engine::myWindow::TakeScreenshot(void)
{
	try
	{
		stbi_flip_vertically_on_write(1);
		char filename[50];
		static char basename[30];
		time_t t = time(NULL);
		strftime(basename, 30, "%Y%m%d_%H%M%S.png", localtime(&t));
		strcpy(filename, "screenshots/");
		strcat(filename, basename);

		int w = (int)myWindow::Width;
		int h = (int)myWindow::Height;
		unsigned char* imageData = new unsigned char[int(w * h * 3)];
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, imageData);

		int saved = stbi_write_png(filename, w, h, 3, imageData, 0);
		free(imageData);
	}
	catch (const std::exception&)
	{
		Engine::GameClose();
	}
}

void Engine::myWindow::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	if (!Settings::FullScreen) glfwWindow = glfwCreateWindow((int)Width, (int)Height, Settings::GetGameName(), nullptr, nullptr); // Windowed
	else glfwWindow = glfwCreateWindow((int)Width, (int)Height, Settings::GetGameName(), glfwGetPrimaryMonitor(), nullptr); // full screen
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

void Engine::myWindow::ClearBuffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwWindowShouldClose(glfwWindow)) {
		ShouldClose = true;
	}
}

void Engine::myWindow::ClearAndSwapBuffers(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(glfwWindow);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::myWindow::SwapBuffers(void)
{
	glfwSwapBuffers(glfwWindow);
}

void Engine::myWindow::create_callbacks() { // important for taking the keyboard / mouse input 
	glfwSetKeyCallback(glfwWindow, handle_keys);
	glfwSetCursorPosCallback(glfwWindow, handle_mouse);
	glfwSetMouseButtonCallback(glfwWindow, mouse_button_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);
	glfwSetCharCallback(glfwWindow, character_callback);
}

void Engine::myWindow::character_callback(GLFWwindow* window, unsigned int codepoint) {
	Keyboard::SetCharCodepointPressed(codepoint);
}

void Engine::myWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) {
	try {
		if (key >= 0 && key < 348) {
			Keyboard::SetKeyStatus(key, action);
		}
		else {
			Logger::LogMessage msg = Logger::LogMessage("The pressed button is not handled. No action will be performed", "Info", "Engine", "myWindow", "handle_keys");
			Logger::Info(msg);
		}
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred while handling keyboard keys.", "Error", "Engine", "myWindow", "handle_keys");
		Logger::Error(msg);
		Engine::GameClose();
	}
}

void Engine::myWindow::handle_mouse(GLFWwindow* window, double xPos, double yPos) {
	lastX = (GLfloat)xPos;
	double y = fabs(yPos - Height);
	lastY = (GLfloat)y;
}

void Engine::myWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	Engine::Mouse::Input(button, action);
}

void Engine::myWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Mouse::ScrollValue = float(yoffset);
	Mouse::ScrollBool = true;
}

Engine::myWindow::~myWindow() {}

