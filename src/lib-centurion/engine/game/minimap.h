/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>
#include <string>

namespace Game {

	// prerendered minimap "image"
	class Minimap
	{
	public:
		/// <summary>
		/// This function create a new minimap.
		/// </summary>
		static void Create(void);

		/// <summary>
		/// This function performs the rendering of a minimap.
		/// </summary>
		static void RenderMapAndObjects(void);

		/// <summary>
		/// This function performs the rendering of the minimap rectangle.
		/// </summary>
		static void RenderRectangle(void);

		/// <summary>
		/// This functions updates the minimap.
		/// </summary>
		static void Update(void);

		/// <summary>
		/// This function checks if the minimap was created.
		/// </summary>
		/// <returns>True or not.</returns>
		static bool IsCreated(void);

		/// <summary>
		/// This function enables the minimap.
		/// </summary>
		static void Enable(void);

		/// <summary>
		/// This function disables the minimap.
		/// </summary>
		static void Disable(void);

		/// <summary>
		/// This function checks if the minimap is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		static bool IsActive(void);

		/// <summary>
		/// This function checks if the minimap is blocked or not.
		/// </summary>
		/// <returns>True if it's blocked; false otherwise.</returns>
		static bool IsBlocked(void);

		/// <summary>
		/// This function blocks the game minimap.
		/// </summary>
		static void Block(void);

		/// <summary>
		/// This function unblocks the game minimap.
		/// </summary>
		static void Unblock(void);
		~Minimap();
	private:
		Minimap();
		static bool isActive;
		static bool isCreated;
		static bool isBlocked;
		static gui::Rectangle minimapRectangle;
	};

};