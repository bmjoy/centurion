#include <engine>
#include <game>

using namespace glb;
using namespace engine;
using namespace game;

myWindow::myWindow(){}

void myWindow::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	MainWindow = glfwCreateWindow((int)getParam("window-width"), (int)getParam("window-height"), gameNameSTR.c_str(), nullptr, nullptr); // Windowed
	//MainWindow = glfwCreateWindow((int)getParam("window-width"), (int)getParam("window-height"), gameNameSTR.c_str(), glfwGetPrimaryMonitor(), nullptr); // full screen
	glfwMakeContextCurrent(MainWindow);

	create_callbacks(); // handle key + mouse input

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);

	// Define the viewport dimensions
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(MainWindow, this); // this function is for taking inputs

	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// hide the mouse cursor in the window
}

void myWindow::clear_buffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (glfwWindowShouldClose(MainWindow)) {
		setBoolean("window-should-close", true);
	}
}

void myWindow::create_callbacks() { // important for taking the keyboard / mouse input 
	glfwSetKeyCallback(MainWindow, handle_keys);
	glfwSetCursorPosCallback(MainWindow, handle_mouse);
	glfwSetMouseButtonCallback(MainWindow, mouse_button_callback);
	glfwSetScrollCallback(MainWindow, scroll_callback);
	glfwSetCharCallback(MainWindow, character_callback);
}

void myWindow::character_callback(GLFWwindow* window, unsigned int codepoint) {
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));
	CharCodepointPressed = (int)codepoint;
}

void myWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) {
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));
	
	KeyCode[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
}

void myWindow::handle_mouse(GLFWwindow* window, double xPos, double yPos){
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));
	theWindow->lastX = (GLfloat)xPos;
	double y = fabs(yPos - getParam("window-height"));
	theWindow->lastY = (GLfloat)y;
}

void myWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			setBoolean("mouse-left", true);
			setBoolean("mouse-release", false);
			setBoolean("mouse-left-pressed", true);
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("mouse-left", false);
			setBoolean("mouse-release", true);
			setBoolean("mouse-left-pressed", false);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			setBoolean("mouse-right", true);
			setBoolean("mouse-release", false);
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("mouse-right", false);
			setBoolean("mouse-release", true);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			setBoolean("mouse-middle", true);
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("mouse-middle", false);
		}
	}
}

void myWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	setParam("mouse-scroll", (float)yoffset);
	setBoolean("mouse-scroll-bool", true);
}

myWindow::~myWindow()
{
}
