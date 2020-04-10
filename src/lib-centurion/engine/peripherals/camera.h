/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef CAMERA_H
#define CAMERA_H

#ifndef CAMERA_THRESHOLD
#define CAMERA_THRESHOLD 20
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Engine 
{
	/// <summary>
	/// This namespace handles the player camera.
	/// </summary>
	namespace Camera 
	{
		/// <summary>
		/// This integer function gets the current zoom value (from 1 to 8).
		/// </summary>
		/// <returns>Current zoom value to return, expressed in integer</returns>
		int GetCurrentZoom(void);

		/// <summary>
		/// This function sets the current zoom value (from 1 to 8).
		/// </summary>
		/// <param name="z">Current zoom value. It supports only const int values</param>
		void SetCurrentZoom(const int z);

		/// <summary>
		/// This floating function gets the current zoom factor, which determines the percentage of enlargement or reduction
		/// for each change of the zoom value.
		/// </summary>
		/// <returns>Current zoom factor percentage to return, expressed in float</returns>
		float GetZoomFactor(void);

		/// <summary>
		/// This function sets the current zoom factor percentage.
		/// </summary>
		/// <param name="f">Current zoom factor percentage. It supports only float values</param>
		void SetZoomFactor(const float f);

		/// <summary>
		/// This floating function returns the current x position of the camera.
		/// </summary>
		/// <returns>Current x position to return, expressd in float</returns>
		float GetXPosition();

		/// <summary>
		/// This floating function gets the current y position of the camera.
		/// </summary>
		/// <returns>Current y position to return, expressd in float</returns>>
		float GetYPosition();

		/// <summary>
		/// This glm::vec2 function calculates the zoomed coordinates of a x,y point.
		/// </summary>
		/// <param name="xCoord">X coordinate. It supports only float values</param>
		/// <param name="yCoord">Y coordinate. It supports only float values</param>
		/// <returns>Current x and y zoomed values to return, expressed in glm::vec2</returns>
		glm::vec2 GetZoomedCoords(const float xCoord, const float yCoord);

		/// <summary>
		/// This floating function gets the y coordinate on minimap of a y point.
		/// </summary>
		/// <param name="y">Y coordinate. It supports only const float values.</param>
		/// <returns>Current y minimap position to return, expressed in float</returns>
		float GetYMinimapCoordinate(const float y);

		/// <summary>
		/// This function creates the camera, initialising all the values
		/// </summary>
		void Create();

		/// <summary>
		/// This function manages camera behavior resulting from mouse interactions.
		/// </summary>
		void mouseControl(void);

		/// <summary>
		/// This function manages camera behavior resulting from keyboard interactions.
		/// </summary>
		void keyboardControl(void);

		/// <summary>
		/// This function move the camera to a specified point instantly.
		/// </summary>
		/// <param name="x">X coordinate to move to. It supports only const GLfloat</param>
		/// <param name="y">Y coordinate to move to. It supports only const GLfloat</param>
		void GoToPoint(const GLfloat x, const  GLfloat y);

		/// <summary>
		/// This function calculates the view matrix.
		/// </summary>
		/// <returns>Current view matrix to return, expressed in glm::vec4</returns>
		glm::mat4 calculateViewMatrix(void);

		// Properties
		extern float MaxZoom;
		extern float MovementSpeed;

	};
};

#endif // !CAMERA_H