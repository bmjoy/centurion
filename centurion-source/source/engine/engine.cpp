#include "engine.h"
#include <file_manager.h>

#include <translationsTable.h>
#include <classes/object-data.h>

#ifndef TIME_LEFT_HOLD 
#define TIME_LEFT_HOLD 0.10f
#endif

#ifndef __MSXML_LIBRARY_DEFINED__
#define __MSXML_LIBRARY_DEFINED__
#endif
#include <Windows.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <GLFW/glfw3.h>

#include <game/game.h>
#include <game/editor.h>
#include <game/strategy.h>
#include <player/player.h>

#include <menu/menu.h>
#include <game/interface/editorWindows.h>

#include <interface>
#include <classes/unit.h>
#include <mapgen/mapgen.h>
#include <settings.h>
#include <errorCodes.h>
#include <logger.h>
#include <hector-lua.h>

// for mouse cursor
#include <cursor_image.h>

// audio
#include "audio_manager.h"

// read data (temporary: waiting for xml reading system)
#include "building_sprite.h"
#include "unit_sprite.h"
#include "decoration_sprite.h"
#include "terrain.h"
#include "image_sprite.h"
// 

using namespace std;
using namespace glm;

using namespace menu;
using namespace debug;

#pragma region Mouse class

// define static variables (default values)
gui::Image Engine::Mouse::img;
vec3 Engine::Mouse::position;
string Engine::Mouse::currentState = "default";
float Engine::Mouse::znoise = 0;
float Engine::Mouse::yzoomed = 0;
float Engine::Mouse::xPosGrid = 0;
float Engine::Mouse::yPosGrid = 0;
float Engine::Mouse::xLeftClick = 0;
float Engine::Mouse::yLeftClick = 0;
float Engine::Mouse::xRightClick = 0;
float Engine::Mouse::yRightClick = 0;
float Engine::Mouse::y2DRightClick = 0;
float Engine::Mouse::y2DPosition = 0;
float Engine::Mouse::ScrollValue = 0;
bool Engine::Mouse::ScrollBool = false;
bool Engine::Mouse::LeftClick = false;
bool Engine::Mouse::RightClick = false;
bool Engine::Mouse::MiddleClick = false;
bool Engine::Mouse::LeftHold = false;
bool Engine::Mouse::RightHold = false;
bool Engine::Mouse::MiddleHold = false;
bool Engine::Mouse::Release = false;
Engine::HoldClickData Engine::Mouse::leftHoldClickData;
Engine::HoldClickData Engine::Mouse::rightHoldClickData;
Engine::HoldClickData Engine::Mouse::middleHoldClickData;
// ------------ end definitions

Engine::Mouse::Mouse() { }

bool Engine::Mouse::IsCursorInGameScreen()
{
	return (GetYLeftClick() > myWindow::BottomBarHeight) && (GetYLeftClick() < (myWindow::Height - myWindow::TopBarHeight));
}

void Engine::Mouse::create() {
	img = gui::Image("circle_pos");
	img.create("center", 0.f, 0.f, 0, 0, 0);
}
void Engine::Mouse::render() {
	Cursor()->render(position.x, position.y, currentState);
	if (Engine::getEnvironment() == STRATEGY_ENV) {
		if (Game::Minimap::IsActive() == false) {
			img.render(false, position.x, y2DPosition);
		}
	}
}
void Engine::Mouse::mouse_control(int lastX, int lastY) {
	position.x = (GLfloat)lastX;
	position.y = (GLfloat)lastY;

	yzoomed = Camera::GetZoomedCoords(position.x, Mouse::GetYPosition()).y;
	znoise = mapgen::smoothNoise(yzoomed, mapgen::mouseZNoise);
	znoise /= myWindow::HeightZoomed / myWindow::Height;

	y2DPosition = Mouse::GetYPosition() - znoise;

	if (!LeftClick) {
		xLeftClick = position.x;
		yLeftClick = position.y;
	}
	else {
		currentState = "left";

		stringstream ss;
		ss << "You have left-clicked on (X=" << (int)xLeftClick << ", Y=" << (int)yLeftClick << ")";
		Logger::Info(ss.str());
	}

	if (!RightClick) {
		xRightClick = position.x;
		yRightClick = position.y;
		y2DRightClick = y2DPosition;
	}
	else {
		currentState = "right";

		stringstream ss;
		ss << "You have right-clicked on (X=" << (int)xRightClick << ", Y=" << (int)yRightClick << ")";
	}

	if (Mouse::Release) {
		currentState = "default";
	}
}

void Engine::Mouse::IsHolding(void)
{
	if (glfwGetTime() - leftHoldClickData.lastTime > TIME_LEFT_HOLD && leftHoldClickData.bIsTimeSaved){
		LeftHold = true;
	}
	if (glfwGetTime() - rightHoldClickData.lastTime > TIME_LEFT_HOLD && rightHoldClickData.bIsTimeSaved) {
		RightHold = true;
	}
	if (glfwGetTime() - middleHoldClickData.lastTime > TIME_LEFT_HOLD && middleHoldClickData.bIsTimeSaved) {
		MiddleHold = true;
	}
}

Engine::Mouse::~Mouse() {}

#pragma endregion

#pragma region Window class

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
			Logger::LogMessage msg = Logger::LogMessage("The pressed button is not handled. No action will be performed", "Info", "", "Engine::myWindow", "handle_keys");
			Logger::Info(msg);
		}
	}
	catch (...) {
		Logger::LogMessage msg = Logger::LogMessage("An error occurred while handling keyboard keys.", "Error", "", "Engine::myWindow", "handle_keys");
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
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			Mouse::LeftClick = true;
			if (Mouse::leftHoldClickData.bIsTimeSaved == false){
				Mouse::leftHoldClickData.lastTime = glfwGetTime();
				Mouse::leftHoldClickData.bIsTimeSaved = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			Mouse::Release = true;
			if (Mouse::leftHoldClickData.bIsTimeSaved) {
				Mouse::leftHoldClickData.bIsTimeSaved = false;
				Mouse::LeftHold = false;
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			Mouse::RightClick = true;
			if (Mouse::rightHoldClickData.bIsTimeSaved == false){
				Mouse::rightHoldClickData.lastTime = glfwGetTime();
				Mouse::rightHoldClickData.bIsTimeSaved = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			Mouse::Release = true;
			if (Mouse::rightHoldClickData.bIsTimeSaved) {
				Mouse::rightHoldClickData.bIsTimeSaved = false;
				Mouse::RightHold = false;
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (action == GLFW_PRESS) {
			Mouse::MiddleClick = true;
			if (Mouse::middleHoldClickData.bIsTimeSaved == false) {
				Mouse::middleHoldClickData.lastTime = glfwGetTime();
				Mouse::middleHoldClickData.bIsTimeSaved = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			Mouse::MiddleClick = false;
			if (Mouse::middleHoldClickData.bIsTimeSaved) {
				Mouse::middleHoldClickData.bIsTimeSaved = false;
				Mouse::MiddleHold = false;
			}
		}
	}
}

void Engine::myWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	Mouse::ScrollValue = float(yoffset);
	Mouse::ScrollBool = true;
}

Engine::myWindow::~myWindow() {}

#pragma endregion

#pragma region Camera class

// define static variables
float Engine::Camera::MaxZoom = 20.f;
float Engine::Camera::MovementSpeed = 10.f;
vec3 Engine::Camera::position, Engine::Camera::front, Engine::Camera::up, Engine::Camera::right, Engine::Camera::worldUp;
GLfloat Engine::Camera::yaw, Engine::Camera::pitch, Engine::Camera::threshold_x, Engine::Camera::threshold_y, Engine::Camera::abs_x, Engine::Camera::abs_y;
float Engine::Camera::zoomCameraFactor = 100.f;
int Engine::Camera::currentZoom = 8;
// ---------- end definitions

Engine::Camera::Camera() {}

vec2 Engine::Camera::GetZoomedCoords(float xCoord, float yCoord)
{
	float x = xCoord * myWindow::WidthZoomed / myWindow::Width + Camera::GetXPosition();
	float y = yCoord * myWindow::HeightZoomed / myWindow::Height + Camera::GetYPosition();
	return vec2(x, y);
}

void Engine::Camera::Init(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch) {
	position = startPosition;

	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, 1.0f);

	update();
}

void Engine::Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Engine::Camera::mouseControl() {

	abs_x = Mouse::GetXPosition() + position.x;
	abs_y = Mouse::GetYPosition() + position.y;

	threshold_x = MEDIUM_MAP_WIDTH - 2 * MovementSpeed + (myWindow::WidthZoomed - myWindow::Width);
	threshold_y = MEDIUM_MAP_HEIGHT - 2 * MovementSpeed + (myWindow::HeightZoomed - myWindow::Height);

	float threshold_top = CAMERA_THRESHOLD;
	if (Engine::getEnvironment() == EDITOR_ENV) threshold_top += 30.f;

	//Left margin
	if (Mouse::GetXPosition() <= CAMERA_THRESHOLD && (abs_x > CAMERA_THRESHOLD) && Mouse::GetXPosition() > 0) {
		position -= right * MovementSpeed;
	}
	//Right margin
	if (Mouse::GetXPosition() >= (myWindow::Width - CAMERA_THRESHOLD) && (abs_x < threshold_x) && Mouse::GetXPosition() < myWindow::Width) {
		if (position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
			position += right * MovementSpeed;
		}
	}
	//Top margin
	if (Mouse::GetYPosition() >= (myWindow::Height - threshold_top) && abs_y < (threshold_y + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::HeightZoomed) && Mouse::GetYPosition() < myWindow::Height) {
		if (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height)) {
			position += up * MovementSpeed;
		}
	}
	//Bottom margin
	if (Mouse::GetYPosition() <= CAMERA_THRESHOLD && abs_y > (CAMERA_THRESHOLD - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height) && Mouse::GetYPosition() > 0) {
		if (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height)) {
			position -= up * MovementSpeed;
		}
	}

	/* MOUSE SCROLLING --> CAMERA ZOOM */
	if (!game::GameMenu::IsActive() && (Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT_CONTROL) || Keyboard::IsKeyNotReleased(GLFW_KEY_RIGHT_CONTROL)) && Mouse::ScrollBool) {
		if (Mouse::ScrollValue > 0 && currentZoom > 1.0f) {
			currentZoom -= (int)Mouse::ScrollValue;
		}

		else if (Mouse::ScrollValue < 0 && currentZoom < MaxZoom) {
			currentZoom -= (int)Mouse::ScrollValue;
		}
		Mouse::ScrollBool = false;
	}

	if (position.x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed - MovementSpeed) {
		position.x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed - MovementSpeed;
	}
	if (position.y < -myWindow::BottomBarHeight * myWindow::HeightZoomed / myWindow::Height) {
		position.y = -myWindow::BottomBarHeight * myWindow::HeightZoomed / myWindow::Height;
	}
	if (position.y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight * myWindow::HeightZoomed / myWindow::Height) {
		position.y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight * myWindow::HeightZoomed / myWindow::Height;
	}
	/* ------------------------------------------- */
	myWindow::WidthZoomed = myWindow::Width + (currentZoom - 1) * zoomCameraFactor;
	myWindow::HeightZoomed = myWindow::Height + (currentZoom - 1) *  zoomCameraFactor / myWindow::Ratio;
}

void Engine::Camera::keyboardControl() {
	//Left margin
	if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT) && position.x > 0) {
		position -= right * MovementSpeed;
	}
	//Right margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_RIGHT) && position.x < MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) {
		position += right * MovementSpeed;
	}
	//Top margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_UP) && (position.y < (MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed + myWindow::TopBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
		position += up * MovementSpeed;
	}
	//Bottom margin 
	if (Keyboard::IsKeyPressed(GLFW_KEY_DOWN) && (position.y > (0 - myWindow::BottomBarHeight* myWindow::HeightZoomed / myWindow::Height))) {
		position -= up * MovementSpeed;
	}
}

void Engine::Camera::GoToPoint(GLfloat x, GLfloat y) {
	if (x < 0.0) { x = 0; }
	else if (x > MEDIUM_MAP_WIDTH - myWindow::WidthZoomed) { x = MEDIUM_MAP_WIDTH - myWindow::WidthZoomed; }
	if (y < 0.0) { y = 0; }
	else if (y > MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed) { y = MEDIUM_MAP_HEIGHT - myWindow::HeightZoomed; }
	position.x = x;
	position.y = y;
}

glm::mat4 Engine::Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

float Engine::Camera::GetYMinimapCoordinate(float y)
{
	return myWindow::Height * (y - myWindow::BottomBarHeight) / (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight);
}

Engine::Camera::~Camera()
{
}

#pragma endregion

#pragma region Keyboard class

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

#pragma endregion

#pragma region Engine class

// define static variables
gui::SimpleText Engine::text;
int Engine::environment = MENU_ENV;
double Engine::currentTime, Engine::lastTime, Engine::finalTime;
int Engine::nbFrames, Engine::Fps, Engine::Mpfs;
bool Engine::reset;
// ---------- end definitions

Engine::Engine() { }

void Engine::GameClose()
{
	myWindow::ShouldClose = true;
}

void Engine::SetEnvironment(string s)
{
	if (s == "menu") {
		environment = MENU_ENV;
	}
	else if (s == "editor") {
		environment = EDITOR_ENV;
	}
	else if (s == "strategy") {
		environment = STRATEGY_ENV;
	}
	else {
		environment = MENU_ENV;
	}
	Mouse::LeftClick = false;
}

void Engine::PrintToConsole(string s)
{
	cout << s << endl;
}

vector<string>* Engine::GetListOfFolders(string s)
{
	if (FileManager::CheckIfFolderExists(s)) {
		vector<string> list = FileManager::GetAllFoldersNamesWithinFolder(s);
		vector<string>* _list = new vector<string>();
		(*_list) = list;
		return _list;
	}
	return nullptr;
}

void Engine::Init(const char* exe_root) {

	// old initParams (global)

	try {

		Settings::SetFolders(FileManager::GetExeFolderPath(exe_root));
		ErrorCodes::ReadErrorCodesXml();
		Logger::CleanLogs();

		Settings::Init();
		Settings::ReadSettings();

		Engine::myWindow::Ratio = Engine::myWindow::Width / Engine::myWindow::Height;
		Engine::myWindow::WidthZoomed = Engine::myWindow::Width + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor();
		Engine::myWindow::HeightZoomed = Engine::myWindow::Height + (Engine::Camera::GetCurrentZoom() - 1) * Engine::Camera::GetZoomFactor() / Engine::myWindow::Ratio;

		setMenuProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::Width, 0.0f, Engine::myWindow::Height, -100.0f, 100.0f));
		setCameraProjectionMatrix(glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH));


		ifstream data_path("assets/data/data.json");
		//Close the game if it wasn't able to find or process data.json file
		if (!data_path.good()) {
			//forceGameClosure("NOT_FOUND", "ERROR_data");
		}
		json data = json::parse(data_path);

		for (int i = 0; i < data["player_colors"].size(); i++) {
			vec3 color = vec3(data["player_colors"][i]["r"], data["player_colors"][i]["g"], data["player_colors"][i]["b"]);
			Game::AddColor(color);
		}


	}
	catch (...) {
		std::cout << "An error occurred" << std::endl;
	}

	//------------------------------

	nbFrames = 0;
	Fps = 0;
	Mpfs = 0;
	SetEnvironment("menu");
	reset = false;
}

int Engine::launch() {
	myWindow window = myWindow::GetInstance();
	init();

	ObjectData::ReadDataClassesFromXml();
	read_data();

	primitives::compile();
	primitives::create();

	lastTime = glfwGetTime();

	Camera::Init(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	Mouse::create();

	DEBUG_UI()->create();

	std::ostringstream ss;
	ss << glGetString(GL_VERSION);
	Logger::Info("Running OpenGL Version " + ss.str());

	// Hector - Lua interpreter
	Hector::Initialize();
	Hector::CreateConsole();

	while (myWindow::ShouldClose == false) {
		glfwPollEvents();
		Mouse::IsHolding(); 
		window.ClearBuffers();
		fps();
		Mouse::mouse_control(window.get_mouse_x(), window.get_mouse_y());
		handleGlobalKeys();

		// ---- MENU ---- //

		if (getEnvironment() == MENU_ENV) {
			
			if (Menu::IsCreated() == false)
			{
				//Audio()->MusicPlay("assets/music/menu.ogg");
				Menu::Create();
				Logger::Info("Main menu was created!");
			}
			Menu::Run();
		}

		// ---- STRATEGY ---- //

		if (getEnvironment() == STRATEGY_ENV) {
			if (!Strategy::IsCreated()) {
				//Audio()->MusicStop();

				window.ClearAndSwapBuffers();

				{
					text = gui::SimpleText("dynamic");
					text.render_dynamic(TranslationsTable::GetTranslation("WORD_gameStarted"), "tahoma_15px", myWindow::Width / 2.f, myWindow::Height / 2.f, vec4(255.f), "center", "middle");
				}

				window.SwapBuffers();

				Strategy::Create();
			}
			Strategy::Run();
		}

		// ---- EDITOR ---- //

		if (getEnvironment() == EDITOR_ENV) {
			if (!Editor::IsCreated()) {
				Audio()->MusicStop();
				Editor::Create();
			}
			Editor::Run();
		}

		// -------------- //

		if (reset) {
			reset = false;
			if (getEnvironment() == EDITOR_ENV) Editor::reset(); editor::clearEditorVariables();
			if (getEnvironment() == STRATEGY_ENV) Strategy::reset();
			//MENU()->reset();

			SetEnvironment("menu");
		}

		// debug ui
		if (Settings::DebugIsActive)	DEBUG_UI()->render(Fps, Mpfs, Unit::GetCounter());

		Hector::RenderConsole();

		// mouse
		Mouse::render();

		if ((Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || Keyboard::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT)) && Keyboard::IsKeyPressed(GLFW_KEY_S)) {
			//Logger::Info("Screenshot taken!");
			//myWindow::TakeScreenshot();
		}

		Keyboard::SetCharCodepointPressed(-1);
		ResetPeriphericsInput();

		window.SwapBuffers();

		fps_sleep();
	}

	//if (MENU()->menu_is_created()) MENU()->reset();
	Logger::SaveParamsXML();
	
	Game::ResetGameObjects();
	Menu::Clear();
	myWindow::DeleteInstance();
	EditorWindows::Clear();

	glfwTerminate();
	return 0;
}

void Engine::read_data() {
	//Read races data
	vector<string> r_files = FileManager::GetAllFilesNamesWithinFolder("assets/data/races");
	for (int i = 0; i < r_files.size(); ++i) {
		ifstream path("assets/data/races/" + r_files[i]);
		json dataRaces = json::parse(path);

		Game::Race r = Game::Race();
		int id = dataRaces["race_id"].get<int>();
		string name = dataRaces["race_name"].get<string>();
		string zone = dataRaces["zone"].get<string>();
		string t_class = dataRaces["food_transport_class"].get<string>();
		r.setRaceProperties(id, name, zone, t_class);
		Game::AddRace(name, r);
	}

	/* images */

	vector<FileManager::file_info> imagesInfoList = FileManager::GetAllFilesNamesWithinSubfolders("assets\\ui", "png");

	for (int i = 0; i < imagesInfoList.size(); i++)
		Img()->addPath(imagesInfoList[i].name, imagesInfoList[i].path);

	/* terrain textures */

	vector<FileManager::file_info> texturesInfoList = FileManager::GetAllFilesNamesWithinSubfolders("assets\\terrain\\textures", "png");

	for (int i = 0; i < texturesInfoList.size(); i++)
		MapTerrain()->addPath(texturesInfoList[i].name, texturesInfoList[i].path);
}

void Engine::handleGlobalKeys() {
	// activate or deactivate debug ui
	if (Settings::DebugIsActive && Keyboard::IsKeyPressed(GLFW_KEY_F10)) {
		debug::DEBUG_UI()->setStatus(!debug::DEBUG_UI()->getStatus());
		debug::DEBUG_UI()->getStatus() ? Logger::Info("Debug UI ON!") : Logger::Info("Debug UI OFF!");
	}
}

void Engine::ResetPeriphericsInput()
{
	//Keyboard buttons
	Keyboard::ResetKeys();

	//Mouse buttons
	Mouse::LeftClick = false;
	Mouse::RightClick = false;
	Mouse::MiddleClick = false;
}

void Engine::fps() {
	currentTime = glfwGetTime();
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		Fps = nbFrames;
		Mpfs = 1000 / nbFrames;
		lastTime += 1.0;
		nbFrames = 0;
	}
}

void Engine::fps_sleep() {
	finalTime = glfwGetTime();
	if (finalTime - currentTime < 1.0 / 60.0) {
		Sleep(DWORD(1000 * (1.0 / 60.0 - (finalTime - currentTime))));
	}
}

Engine::~Engine() { }

#pragma endregion


