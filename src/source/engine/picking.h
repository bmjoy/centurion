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

#include <header.h>

/// <summary>
/// This class handles the picking ID.
/// (???) Molte di queste funzioni dovrebbero controllare che il picking ID sia effettivamente valido.
/// </summary>
class Picking
{
public: 
	/// <summary>
	/// This struct handles the double click.
	/// (???) Da rivedere. Renderla privata cosi come le variabili seguenti per rispettare il principio dell'incapsulamento.
	/// </summary>
	struct DoubleClickData 
	{
		double lastTime = 0;
		int clickCount = 0;
		int clickIdList[2] = { 0, 0 };
	};
	static unsigned int leftClickID;
	static unsigned int leftClickID_UI;
	static unsigned int rightClickID;
	static DoubleClickData doubleClickData;

	/// <summary>
	/// This function returns the picking ID of an object when you click on it.
	/// </summary>
	/// <param name="LeftRight">A value indicating if you are clicking by the left mouse button or the right button.</param>
	/// <returns></returns>
	static unsigned int GetIdFromClick(const unsigned short int LeftRight = PICKING_LEFT);
	/// <summary>
	/// This function returns the color of an object by its picking ID.
	/// </summary>
	/// <param name="par_pickingID">The picking Id of the object. </param>
	/// <returns></returns>
	static glm::vec3 GetPickingColorFromID(const unsigned int par_pickingID);
	/// <summary>
	/// This function resets the time that indicate when there was a double click.
	/// </summary>
	static void ResetDoubleClickTime(void);
	/// <summary>
	/// THis function checks if there was a double click on an object.
	/// </summary>
	/// <returns>True or false.</returns>
	static bool HasDoubleClicked(void);
	/// <summary>
	/// The destructor.
	/// </summary>
	~Picking(void);

private:
	/// <summary>
	/// The constructor.
	/// </summary>
	Picking(void);
};

/// <summary>
/// This function handles the picking ID of an element belongs to UI.
/// </summary>
class PickingUI : public Picking
{
public:
	/// <summary>
	/// This functions assigns a picking ID to a UI element.
	/// </summary>
	/// <returns>A picking ID.</returns>
	static unsigned int ObtainPickingID(void);
	/// <summary>
	/// This function returs the last picking ID that was assigned to a UI element. 
	/// </summary>
	/// <returns>The last picking ID that was assigned to a UI element.</returns>
	static unsigned int GetLastPickingID(void);
	/// <summary>
	/// This function resets all the picking ID, namely all picking ID are now available. 
	/// </summary>
	static void ResetPicking(void);
	/// <summary>
	/// This function adds an UI element to the picking ID list.
	/// </summary>
	/// <param name="picking_id">The picking ID of the UI element.</param>
	/// <param name="par_value">The name of the UI element.</param>
	static void AddValueToPickingList(const unsigned int picking_id, const std::string par_value);
	/// <summary>
	/// This function returns the name of an UI element through its picking ID. 
	/// </summary>
	/// <param name="par_pickingID_UI">The picking ID of an UI element.</param>
	/// <returns>The name of an UI element.</returns>
	static std::string GetPickedObjectName(const unsigned int par_pickingID_UI);
	~PickingUI();
private:
	static unsigned int pickingID_UI;
	static std::map<unsigned int, std::string> pickingList_UI;
	PickingUI();
};

/// <summary>
/// This function handles the picking ID of an element belongs to objects.
/// </summary>
class PickingObject : public Picking
{
public:
	/// <summary>
	/// This functions assigns a picking ID to an object.
	/// It can reuse an old picking ID that belonged to a deleted object (for example a died unit).
	/// </summary>
	/// <returns>A picking ID.</returns>
	static unsigned int ObtainPickingID(void);
	/// <summary>
	/// This function marks a picking ID as reusable during the assignment phase. 
	/// </summary>
	/// <param name="par_pickingID">The picking ID.</param>
	static void AddUnsedPickingID(const unsigned int par_pickingID);
	/// <summary>
	/// This function returs the last picking ID that was assigned to an object.
	/// </summary>
	/// <returns>The last picking ID that was assigned to a UI element.</returns>
	static unsigned int GetLastPickingID(void);
	/// <summary>
	/// This function resets all the picking ID, namely all picking ID are now available. 
	/// </summary>
	static void ResetPicking(void);
	~PickingObject(void);
private:
	static unsigned int pickingID_Object;
	static std::vector<unsigned int> unsedPickingID;
	PickingObject(void);
};
