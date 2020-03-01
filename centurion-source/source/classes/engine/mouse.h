#ifndef MOUSE_H
#define MOUSE_H

#include <gui>

using namespace std;
using namespace glm;

namespace engine {

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
};

#endif