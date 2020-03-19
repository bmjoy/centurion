#ifndef ENGINE_H
#define ENGINE_H

#include <ui.h>

using namespace std;
using namespace glm;

struct GLFWwindow;

class Engine {
public:

	class Camera {

#define CAMERA_THRESHOLD 20

	public:
		static float MaxZoom;
		static float MovementSpeed;
		static int GetCurrentZoom() { return currentZoom; }
		static void SetCurrentZoom(int z) { currentZoom = z; }
		static float GetZoomFactor() { return zoomCameraFactor; }
		static void SetZoomFactor(float f) { zoomCameraFactor = f; }
		static float GetXPosition() { return position.x; }
		static float GetYPosition() { return position.y; }
		static vec2 GetZoomedCoords(float xCoord, float yCoord);
		static float GetYMinimapCoordinate(float y);
		static void Init(vec3 startPosition, vec3 startUp, GLfloat startYaw, GLfloat startPitch);
		static void mouseControl();
		static void keyboardControl();
		static void GoToPoint(GLfloat x, GLfloat y);
		static mat4 calculateViewMatrix();
	private:
		Camera();
		~Camera();
		static vec3 position;
		static vec3 front, up, right, worldUp;
		static GLfloat yaw, pitch;
		static float threshold_x, threshold_y, abs_x, abs_y;
		static float zoomCameraFactor;
		static int currentZoom;
		static void update();
	};

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

		static myWindow GetInstance(void);
		static void DeleteInstance(void);
		static void TakeScreenshot(void);
		

		GLFWwindow* GetGlfwWindow() { return glfwWindow; }
		void ClearBuffers();
		void ClearAndSwapBuffers(void);
		void SwapBuffers(void);
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

	struct HoldClickData
	{
		double lastTime = 0;
		bool bIsTimeSaved = false;
	};
	
	class Mouse
	{
	public:
		static HoldClickData leftHoldClickData;
		static HoldClickData rightHoldClickData;
		static HoldClickData middleHoldClickData;

		Mouse();
		static float ScrollValue;
		static bool ScrollBool;
		static float GetXPosition() { return position.x; }
		static float GetYPosition() { return position.y; }
		static float GetY2DPosition() { return y2DPosition; }
		static float GetXLeftClick() { return xLeftClick; }
		static float GetYLeftClick() { return yLeftClick; }
		static float GetXRightClick() { return xRightClick; }
		static float GetYRightClick() { return yRightClick; }
		static float GetY2DRightClick() { return y2DRightClick; }
		static bool IsCursorInGameScreen();
		static bool LeftClick;
		static bool RightClick;
		static bool MiddleClick;
		static bool LeftHold;
		static bool RightHold;
		static bool MiddleHold;
		static bool Release;
		static void create();
		static void render();
		static void mouse_control(int lastX, int lastY);
		static void IsHolding(void);
		~Mouse();

	private:
		static gui::Image img;
		static vec3 position;
		static string currentState;
		static float znoise, yzoomed, xPosGrid, yPosGrid, xLeftClick, yLeftClick, xRightClick, yRightClick, y2DPosition, y2DRightClick;
	};

	class Keyboard {
	public:
		static void SetKeyStatus(unsigned int key_code, int action) { keyCode[key_code] = action; }
		static bool IsKeyReleased(unsigned int key_code) { return keyCode[key_code]==0; }
		static bool IsKeyPressed(unsigned int key_code) { return keyCode[key_code]==1; }
		static bool IsKeyHold(unsigned int key_code) { return keyCode[key_code]==2; }
		static bool IsKeyNotReleased(unsigned int key_code) { return keyCode[key_code] != 0; }
		static int GetCharCodepointPressed(void) { return charCodepointPressed; }
		static void SetCharCodepointPressed(int codepoint);
		static void ResetKeys() { std::fill(keyCode, keyCode + 348, 0); }
	private:
		static int keyCode[348];
		static int charCodepointPressed;
	};

#define MENU_ENV 0
#define EDITOR_ENV 1
#define STRATEGY_ENV 2

#pragma region TO-LUA Methods

	static void GameClose();
	static void SetEnvironment(string s);
	static void PrintToConsole(string s);
	static vector<string> *GetListOfFolders(string s);

#pragma endregion
	   	 
	static void Init(const char* exe_root);
	static void Reset() { reset = true; }
	static int getEnvironment() { return environment; }
	static int launch();
	~Engine();

private:
	Engine();
	// objects 
	static gui::SimpleText text;
	static int environment;

	// fps
	static double currentTime, lastTime, finalTime;
	static int nbFrames, Fps, Mpfs;
	static bool reset;

	// Private methods
	static void fps();
	static void fps_sleep();
	static void read_data();
	static void handleGlobalKeys();
	static void ResetPeriphericsInput();
};

#endif