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
	glClear(GL_COLOR_BUFFER_BIT);

	if (glfwWindowShouldClose(MainWindow)) {
		setBoolean("window-should-close", true);
	}
}

void myWindow::create_callbacks() { // important for taking the keyboard / mouse input 
	glfwSetKeyCallback(MainWindow, handle_keys);
	glfwSetCursorPosCallback(MainWindow, handle_mouse);
	glfwSetMouseButtonCallback(MainWindow, mouse_button_callback);
	glfwSetScrollCallback(MainWindow, scroll_callback);
}

void myWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE) {
		if (action == GLFW_PRESS) {
			setBoolean("esc-key", true);
			if (ENGINE()->getEnvironment() == "game") {
				gameMenuStatus = !gameMenuStatus;
			}
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("esc-key", false);
		}
	}

	//Arrow keys detection
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			setBoolean("up-key", true);
		}
		if (action == GLFW_RELEASE) {
			setBoolean("up-key", false);
		}
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			setBoolean("down-key", true);
		}
		if (action == GLFW_RELEASE) {
			setBoolean("down-key", false);
		}
	}
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			setBoolean("left-key", true);
		}
		if (action == GLFW_RELEASE) {
			setBoolean("left-key", false);
		}
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			setBoolean("right-key", true);
		}
		if (action == GLFW_RELEASE) {
			setBoolean("right-key", false);
		}
	}
	// Wireframe
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		setBoolean("wireframe", !getBoolean("wireframe"));
		if (getBoolean("wireframe")) {
			std::cout << "DEBUG: Wireframe ON!\n";
		}
		else {
			std::cout << "DEBUG: Wireframe OFF! \n";
		}
	}
	// Grid
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		gameGridStatus = !gameGridStatus;
		if (gameGridStatus) {
			std::cout << "DEBUG: Grid ON!\n";
		}
		else {
			std::cout << "DEBUG: Grid OFF!\n";
		}
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !gameMenuStatus) {
		gameMinimapStatus = !gameMinimapStatus;
		if (gameMinimapStatus) {
			std::cout << "DEBUG: Minimap camera ON!\n";
		}
		else {
			std::cout << "DEBUG: Minimap camera OFF!\n";
		}
	}
	if (key >= 0 && key < 1024) {  // is the key valid? true/false
		if (action == GLFW_PRESS) {
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			theWindow->keys[key] = false;
		}
	}
	if ((key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)) {
		if (action == GLFW_PRESS) {
			setBoolean("ctrl-key", true);
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("ctrl-key", false);
		}
	}
	
}

void myWindow::handle_mouse(GLFWwindow* window, double xPos, double yPos)
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));
	theWindow->lastX = (GLfloat)xPos;
	double y = fabs(yPos - getParam("window-height"));
	theWindow->lastY = (GLfloat)y;
}

void myWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			setBoolean("mouse-left", true);
			setBoolean("mouse-release", false);
			if (ENGINE()->getEnvironment() != "menu") setBoolean("mouse-left-pressed", true);
		}
		else if (action == GLFW_RELEASE) {
			setBoolean("mouse-left", false);
			setBoolean("mouse-release", true);
			if (ENGINE()->getEnvironment() != "menu") setBoolean("mouse-left-pressed", false);
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
	if (ENGINE()->getEnvironment() == "game") {
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			gameMinimapStatus = !gameMinimapStatus;
			if (gameMinimapStatus) {
				std::cout << "DEBUG: Minimap camera ON!\n";
			}
			else {
				std::cout << "DEBUG: Minimap camera OFF!\n";
			}
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
