/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef MOUSE_H
#define MOUSE_H

#ifndef TIME_LEFT_HOLD 
#define TIME_LEFT_HOLD 0.10f
#endif

#ifndef CURSOR_TYPE_DEFAULT
#define CURSOR_TYPE_DEFAULT 0
#endif

#ifndef CURSOR_TYPE_CIRCLE
#define CURSOR_TYPE_CIRCLE 1
#endif

namespace Engine 
{
	/// <summary>
	/// This namespace handles the mouse.
	/// </summary>
	namespace Mouse
	{
		/// <summary>
		/// This function creates an instance of the mouse class.
		/// </summary>
		void Create(void);

		/// <summary>
		/// This function performs the render of the cursor.
		/// </summary>
		void Render(void);

		/// <summary>
		/// This function gets the x coordinate of the mouse.
		/// </summary>
		/// <returns>This value it's true if the specified button is begin released; it's false otherwise</returns>
		float GetXPosition(void);

		/// <summary>
		/// This function gets the y coordinate of the mouse.
		/// </summary>
		float GetYPosition(void);

		/// <summary>
		/// 
		/// </summary>
		float GetY2DPosition(void);

		/// <summary>
		/// 
		/// </summary>
		float GetXLeftClick(void);

		/// <summary>
		/// 
		/// </summary>
		float GetYLeftClick(void);

		/// <summary>
		/// 
		/// </summary>
		float GetXRightClick(void);

		/// <summary>
		/// 
		/// </summary>
		float GetYRightClick(void);

		/// <summary>
		/// 
		/// </summary>
		float GetY2DRightClick(void);

		/// <summary>
		/// 
		/// </summary>
		float GetXMapCoordinate(void);

		/// <summary>
		/// 
		/// </summary>
		float GetYMapCoordinate(void);

		/// <summary>
		/// 
		/// </summary>
		bool IsCursorInGameScreen(void);

		/// <summary>
		/// This function sets the mouse Z noise, useful for mouse-Y-2D-position calculation
		/// </summary>
		void SetMouseZNoise(float z);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="lastX"></param>
		/// <param name="lastY"></param>
		void Control();

		/// <summary>
		/// 
		/// </summary>
		void IsHolding(void);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="button"></param>
		/// <param name="action"></param>
		void Input(const int button, const int action);

		/// <summary>
		///  This method changes the cursor type
		/// </summary>
		/// <param name="type">This value indicates the type. Accepted values are CURSOR_TYPE_DEFAULT or CURSOR_TYPE_CIRCLE</param>
		void ChangeCursorType(const int type = CURSOR_TYPE_DEFAULT);

		// Properties
		extern bool LeftClick;
		extern bool RightClick;
		extern bool MiddleClick;
		extern bool LeftHold;
		extern bool RightHold;
		extern bool MiddleHold;
		extern bool Release;
		extern float ScrollValue;
		extern bool ScrollBool;

	};
};

#endif // !MOUSE_H