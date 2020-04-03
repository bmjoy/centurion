#ifndef MOUSE_H
#define MOUSE_H

#ifndef TIME_LEFT_HOLD 
#define TIME_LEFT_HOLD 0.10f
#endif

#include <ui.h>

namespace Engine {

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
		static float GetXMapCoordinate();
		static float GetYMapCoordinate();
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
		static glm::vec3 position;
		static int currentState;
		static float znoise, yzoomed, xPosGrid, yPosGrid, xLeftClick, yLeftClick, xRightClick, yRightClick, y2DPosition, y2DRightClick;
	};

};

#endif // !MOUSE_H