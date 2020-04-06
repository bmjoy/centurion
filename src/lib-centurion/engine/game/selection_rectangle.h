/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>

namespace Game {

	/// <summary>
	/// This class allow the multiple selection using a rectangle.
	/// </summary>
	class SelectionRectangle
	{
	public:
		/// <summary>
		/// The coordinates of the rectangular selection.
		/// </summary>
		struct SelRectPoints
		{
			float startX, startY, lastX, lastY;
			float minX, maxX, minY, maxY;
		};
		static SelRectPoints Coordinates;
		static float cameraLastX, cameraLastY;

		/// <summary>
		/// (???) Cosa fa?
		/// </summary>
		/// <param name="coords"></param>
		/// <returns></returns>
		static bool IsInRectangle(std::array<float, 8> &coords);

		/// <summary>
		/// This function creates a rectangula selection.
		/// </summary>
		static void Create(void);

		/// <summary>
		/// This function performs the rende
		/// </summary>
		static void Render(void);

		/// <summary>
		/// This function checks if the rectaungular selection is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		static bool IsActive(void);

		/// <summary>
		/// This function enables the rectangula selection.
		/// </summary>
		static void Enable(void);

		/// <summary>
		/// This function disables the rectangular selection.
		/// </summary>
		static void Disable(void);

	private:
		/// <summary>
		/// This method resets the minX, maxX, minY, maxY
		/// </summary>
		static void ResetExtremeValues(void);

		/// <summary>
		/// This method sets the minX, maxX, minY, maxY using the rectangle coordinates data
		/// </summary>
		static void SetExtremeValues(void);

		/// <summary>
		/// This method calculates the size and the origin of the rectangle
		/// </summary>
		static void CalculateSizeAndOrigin(float *width, float *height, int *origin);

		SelectionRectangle(void);
		static gui::Rectangle selRectangle;
		static bool isActive;
	};



};