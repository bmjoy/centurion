#ifndef ENGINE_H
#define ENGINE_H

#include <gui>

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

	class Mouse
	{
	public:
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
		static bool LeftClick;
		static bool RightClick;
		static bool LeftHold;
		static bool MiddleClick;
		static bool Release;
		static void create();
		static void render();
		static void mouse_control(int lastX, int lastY);
		~Mouse();
	private:
		static gui::Image img;
		static vec3 position;
		static string currentState;
		static float znoise, yzoomed, xPosGrid, yPosGrid, xLeftClick, yLeftClick, xRightClick, yRightClick, y2DPosition, y2DRightClick;
	};

	class Keyboard {
	public:
		static void SetKeyStatus(unsigned int key_code, bool condition) { keyCode[key_code] = condition; }
		static bool IsKeyPressed(unsigned int key_code) { return keyCode[key_code]; }
		static int GetCharCodepointPressed(void) { return charCodepointPressed; }
		static void SetCharCodepointPressed(int codepoint);
	private:
		static bool keyCode[348];
		static int charCodepointPressed;
	};

	static void Init();
	static void Reset() { reset = true; }
	static void setEnvironment(string s) { environment = s; }
	static string getEnvironment() { return environment; }
	static int launch();
	~Engine();

private:
	Engine();
	// objects 
	static gui::SimpleText text;
	static string environment;

	// fps
	static double currentTime, lastTime, finalTime;
	static int nbFrames, Fps, Mpfs;
	static bool reset;

	// Private methods
	static void resetKeyCodes();
	static void fps();
	static void fps_sleep();
	static void read_data();
	static void handleGlobalKeys();
};

#endif