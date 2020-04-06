#ifndef MOUSE_H
#define MOUSE_H

#ifndef TIME_LEFT_HOLD 
#define TIME_LEFT_HOLD 0.10f
#endif

namespace Engine 
{
	namespace Mouse
	{
		/// <summary>
		/// 
		/// </summary>
		void Create(void);

		/// <summary>
		/// 
		/// </summary>
		void Render(void);

		/// <summary>
		/// 
		/// </summary>
		float GetXPosition(void);

		/// <summary>
		/// 
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
		/// 
		/// </summary>
		/// <param name="lastX"></param>
		/// <param name="lastY"></param>
		void Control(const int lastX, const int lastY);

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