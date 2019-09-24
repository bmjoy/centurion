#include "window.h"

myWindow::myWindow()
{
}

void myWindow::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	mainWindow = glfwCreateWindow(GLB::WINDOW_WIDTH, GLB::WINDOW_HEIGHT, "Centurion", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(mainWindow);

	create_callbacks(); // handle key + mouse input

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Define the viewport dimensions
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this); // this function is for taking inputs

	// Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// hide the mouse cursor in the window
}

void myWindow::clear_buffers() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (glfwWindowShouldClose(mainWindow)) {
		GLB::WINDOW_CLOSE = true;
	}
}

void myWindow::create_callbacks() { // important for taking the keyboard / mouse input 
	glfwSetKeyCallback(mainWindow, handle_keys);
	glfwSetCursorPosCallback(mainWindow, handle_mouse);
	glfwSetMouseButtonCallback(mainWindow, mouse_button_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);
}

void myWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) 
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE) {
		if (action == GLFW_PRESS) {
			GLB::ESC_KEY = true;
			if (GLB::GAME) GAME::MENU_IS_ACTIVE = !GAME::MENU_IS_ACTIVE;
		}
		else if (action == GLFW_RELEASE) {
			GLB::ESC_KEY = false;
		}
	}

	//Arrow keys detection
	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			GLB::UP_KEY = true;
		}
		if (action == GLFW_RELEASE) {
			GLB::UP_KEY = false;
		}
	}
	if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			GLB::DOWN_KEY = true;
		}
		if (action == GLFW_RELEASE) {
			GLB::DOWN_KEY = false;
		}
	}
	if (key == GLFW_KEY_LEFT) {
		if (action == GLFW_PRESS) {
			GLB::LEFT_KEY = true;
		}
		if (action == GLFW_RELEASE) {
			GLB::LEFT_KEY = false;
		}
	}
	if (key == GLFW_KEY_RIGHT) {
		if (action == GLFW_PRESS) {
			GLB::RIGHT_KEY = true;
		}
		if (action == GLFW_RELEASE) {
			GLB::RIGHT_KEY = false;
		}
	}
	// Wireframe
	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		GLB::WIREFRAME = !GLB::WIREFRAME;
		if (GLB::WIREFRAME) {
			std::cout << "Wireframe ON! \n";
		}
		else {
			std::cout << "Wireframe OFF! \n";
		}
	}
	// Grid
	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		GAME::GRID_IS_ACTIVE = !GAME::GRID_IS_ACTIVE;
		if (GAME::GRID_IS_ACTIVE) {
			std::cout << "Grid ON! \n";
		}
		else {
			std::cout << "Grid OFF! \n";
		}
	}
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !GAME::MENU_IS_ACTIVE) {
		 GAME::MINIMAP_IS_ACTIVE = ! GAME::MINIMAP_IS_ACTIVE;
		if ( GAME::MINIMAP_IS_ACTIVE) {
			std::cout << "Minimap camera ON! \n";
		}
		else {
			std::cout << "Minimap camera OFF! \n";
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
			GLB::CTRL_BUTTON = true;
		}
		else if (action == GLFW_RELEASE) {
			GLB::CTRL_BUTTON = false;
		}
	}
	
}

void myWindow::handle_mouse(GLFWwindow* window, double xPos, double yPos)
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));
	theWindow->lastX = xPos;
	double y = fabs(yPos - GLB::WINDOW_HEIGHT);
	theWindow->lastY = y;
}

void myWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	myWindow* theWindow = static_cast<myWindow*>(glfwGetWindowUserPointer(window));

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			GLB::MOUSE_LEFT = true;
			GLB::MOUSE_RELEASE = false;
		}
		else if (action == GLFW_RELEASE) {
			GLB::MOUSE_LEFT = false;
			GLB::MOUSE_RELEASE = true;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			GLB::MOUSE_RIGHT = true;
			GLB::MOUSE_RELEASE = false;
		}
		else if (action == GLFW_RELEASE) {
			GLB::MOUSE_RIGHT = false;
			GLB::MOUSE_RELEASE = true;
		}
	}
	if (GLB::GAME){
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			 GAME::MINIMAP_IS_ACTIVE =! GAME::MINIMAP_IS_ACTIVE;
			if (GAME::MINIMAP_IS_ACTIVE) {
				std::cout << "Minimap camera ON! \n";
			}
			else {
				std::cout << "Minimap camera OFF! \n";
			}
		}
	}
}

void myWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	GLB::MOUSE_SCROLL = yoffset;
	GLB::MOUSE_SCROLL_BOOL = true;
}

myWindow::~myWindow()
{
}
