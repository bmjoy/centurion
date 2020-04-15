/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <array>

namespace Game {

	/// <summary>
	/// This class allow the multiple selection using a rectangle.
	/// </summary>
	namespace SelectionRectangle
	{
		/// <summary>
		/// (???) Cosa fa?
		/// </summary>
		/// <param name="coords"></param>
		/// <returns></returns>
		bool IsInRectangle(std::array<float, 8> &coords);

		/// <summary>
		/// This function creates a rectangula selection.
		/// </summary>
		void Create(void);

		/// <summary>
		/// This function performs the rende
		/// </summary>
		void Render(void);

		/// <summary>
		/// This function checks if the rectaungular selection is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		bool IsActive(void);

		/// <summary>
		/// This function enables the rectangula selection.
		/// </summary>
		void Enable(void);

		/// <summary>
		/// This function disables the rectangular selection.
		/// </summary>
		void Disable(void);

		/// <summary>
		/// This method resets the minX, maxX, minY, maxY
		/// </summary>
		void ResetExtremeValues(void);

		/// <summary>
		/// This method sets the minX, maxX, minY, maxY using the rectangle coordinates data
		/// </summary>
		void SetExtremeValues(void);

		/// <summary>
		/// This method sets the startX, startY, lastX and lastY
		/// </summary>
		void ResetCoordinates(void);

		/// <summary>
		/// This method calculates the size and the origin of the rectangle
		/// </summary>
		void CalculateSizeAndOrigin(float *width, float *height, int *origin);
		
	};
};
