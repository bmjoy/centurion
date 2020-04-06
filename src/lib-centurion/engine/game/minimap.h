/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

namespace Game {

	// prerendered minimap "image"
	namespace Minimap
	{

		/// <summary>
		/// This function create a new minimap.
		/// </summary>
		void Create(void);

		/// <summary>
		/// This function performs the rendering of a minimap.
		/// </summary>
		void RenderMapAndObjects(void);

		/// <summary>
		/// This function performs the rendering of the minimap rectangle.
		/// </summary>
		void RenderRectangle(void);

		/// <summary>
		/// This functions updates the minimap.
		/// </summary>
		void Update(void);

		/// <summary>
		/// This function checks if the minimap was created.
		/// </summary>
		/// <returns>True or not.</returns>
		bool IsCreated(void);

		/// <summary>
		/// This function enables the minimap.
		/// </summary>
		void Enable(void);

		/// <summary>
		/// This function disables the minimap.
		/// </summary>
		void Disable(void);

		/// <summary>
		/// This function checks if the minimap is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		bool IsActive(void);

		/// <summary>
		/// This function checks if the minimap is blocked or not.
		/// </summary>
		/// <returns>True if it's blocked; false otherwise.</returns>
		bool IsBlocked(void);

		/// <summary>
		/// This function blocks the game minimap.
		/// </summary>
		void Block(void);

		/// <summary>
		/// This function unblocks the game minimap.
		/// </summary>
		void Unblock(void);

	};
};