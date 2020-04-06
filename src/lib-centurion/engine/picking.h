/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#define PICKING_ID_MIN 1
#define PICKING_ID_MAX 16777215
#define PICKING_LEFT 1
#define PICKING_RIGHT 2
#define PICKING_TERRAIN_ID 0

#include <header.h>

/// <summary>
/// This class handles the picking ID.
/// (???) Molte di queste funzioni dovrebbero controllare che il picking ID sia effettivamente valido.
/// </summary>
namespace Picking
{
	/// <summary>
	/// This function returns the color of an object by its picking ID.
	/// </summary>
	/// <param name="par_pickingID">The picking Id of the object. </param>
	/// <returns></returns>
	glm::vec3 GetPickingColorFromID(const unsigned int par_pickingID);

	/// <summary>
	/// This function resets the time that indicate when there was a double click.
	/// </summary>
	void ResetDoubleClickTime(void);

	/// <summary>
	/// THis function checks if there was a double click on an object.
	/// </summary>
	/// <returns>True or false.</returns>
	bool HasDoubleClicked(void);

	/// <summary>
	/// This function returns the picking ID of an object when you click on it.
	/// </summary>
	/// <param name="LeftRight">A value indicating if you are clicking by the left mouse button or the right button.</param>
	/// <returns></returns>
	unsigned int GetIdFromClick(const unsigned short int LeftRight = PICKING_LEFT);

	/// <summary>
	/// This function handles the picking ID of an element belongs to UI.
	/// </summary>
	namespace UI {
		/// <summary>
		/// This method resets the leftclick and rightclick ids
		/// </summary>
		void ResetClickIds(void);

		/// <summary>
		/// This method returns the leftclick id;
		/// </summary>
		/// <returns></returns>
		unsigned int GetLeftClickId(void);

		/// <summary>
		/// This method returns the rightclick id;
		/// </summary>
		/// <returns></returns>
		unsigned int GetRightClickId(void);

		/// <summary>
		/// This method updates the leftclick and rightclick ids
		/// </summary>
		void UpdateClickIds(void);

		/// <summary>
		/// This functions assigns a picking ID to a UI element.
		/// </summary>
		/// <returns>A picking ID.</returns>
		unsigned int ObtainPickingID(void);

		/// <summary>
		/// This function returs the last picking ID that was assigned to a UI element. 
		/// </summary>
		/// <returns>The last picking ID that was assigned to a UI element.</returns>
		unsigned int GetLastPickingID(void);

		/// <summary>
		/// This function resets all the picking ID, namely all picking ID are now available. 
		/// </summary>
		void ResetPicking(void);
	};

	/// <summary>
	/// This function handles the picking ID of an element belongs to objects.
	/// </summary>
	namespace Obj
	{
		/// <summary>
		/// This method resets the leftclick and rightclick ids
		/// </summary>
		void ResetClickIds(void);

		/// <summary>
		/// This method returns the leftclick id;
		/// </summary>
		/// <returns></returns>
		unsigned int GetLeftClickId(void);

		/// <summary>
		/// This method returns the rightclick id;
		/// </summary>
		/// <returns></returns>
		unsigned int GetRightClickId(void);

		/// <summary>
		/// This method updates the leftclick and rightclick ids
		/// </summary>
		void UpdateClickIds(void);

		/// <summary>
		/// This functions assigns a picking ID to an object.
		/// It can reuse an old picking ID that belonged to a deleted object (for example a died unit).
		/// </summary>
		/// <returns>A picking ID.</returns>
		unsigned int ObtainPickingID(void);

		/// /// <summary>
		/// This function marks a picking ID as reusable during the assignment phase. 
		/// </summary>
		/// <param name="par_pickingID">The picking ID.</param>
		void AddUnsedPickingID(const unsigned int par_pickingID);

		/// <summary>
		/// This function returs the last picking ID that was assigned to an object.
		/// </summary>
		/// <returns>The last picking ID that was assigned to a UI element.</returns>
		unsigned int GetLastPickingID(void);

		/// <summary>
		/// This function resets all the picking ID, namely all picking ID are now available. 
		/// </summary>
		void ResetPicking(void);
	};
};


