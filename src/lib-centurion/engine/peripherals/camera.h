#ifndef CAMERA_H
#define CAMERA_H

#ifndef CAMERA_THRESHOLD
#define CAMERA_THRESHOLD 20
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Engine 
{
	namespace Camera 
	{
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		int GetCurrentZoom(void);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="z"></param>
		void SetCurrentZoom(const int z);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetZoomFactor(void);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="f"></param>
		void SetZoomFactor(const float f);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetXPosition();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetYPosition();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="xCoord"></param>
		/// <param name="yCoord"></param>
		/// <returns></returns>
		glm::vec2 GetZoomedCoords(const float xCoord, const float yCoord);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="y"></param>
		/// <returns></returns>
		float GetYMinimapCoordinate(const float y);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="startPosition"></param>
		/// <param name="startUp"></param>
		/// <param name="startYaw"></param>
		/// <param name="startPitch"></param>
		void Init(const glm::vec3 startPosition, const glm::vec3 startUp, const GLfloat startYaw, const GLfloat startPitch);

		/// <summary>
		/// 
		/// </summary>
		void mouseControl(void);

		/// <summary>
		/// 
		/// </summary>
		void keyboardControl(void);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		void GoToPoint(const GLfloat x, const  GLfloat y);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		glm::mat4 calculateViewMatrix(void);


		// Properties
		extern float MaxZoom;
		extern float MovementSpeed;

	};

};

#endif // !CAMERA_H