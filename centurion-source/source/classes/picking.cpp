#include <picking.h>

#include <engine.h>


#pragma region Picking Class

/* static variables */
unsigned int Picking::leftClickID = 0;
unsigned int Picking::leftClickID_UI = 0;
unsigned int Picking::rightClickID = 0;
Picking::DoubleClickData Picking::doubleClickData;


unsigned int Picking::GetIdFromClick(const unsigned short int LeftRight)
{
	unsigned char data[4];
	//Edit the following line because you can get id with both left and right click
	if (LeftRight == PICKING_LEFT)
		glReadPixels((GLint)Engine::Mouse::GetXLeftClick(), (GLint)Engine::Mouse::GetYLeftClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	if (LeftRight == PICKING_RIGHT)
		glReadPixels((GLint)Engine::Mouse::GetXRightClick(), (GLint)Engine::Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	unsigned int pickedID =
		         data[0] +
		         data[1] * 256 +
		         data[2] * 256 * 256;
	return pickedID;
}

vec3 Picking::getPickingColorFromID(const unsigned int par_pickingID)
{
	unsigned int r = (par_pickingID & 0x000000FF) >> 0;
	unsigned int g = (par_pickingID & 0x0000FF00) >> 8;
	unsigned int b = (par_pickingID & 0x00FF0000) >> 16;
	return vec3(r, g, b);
}

void Picking::resetDoubleClickTime(void)
{
	doubleClickData.lastTime = glfwGetTime();
}

bool Picking::hasDoubleClicked(void)
{
	bool output = false;

	if (glfwGetTime() - doubleClickData.lastTime > 1.0f)
	{
		doubleClickData.clickCount = 0;
	}
	if (doubleClickData.clickCount == 0)
	{
		doubleClickData.clickCount++;
		doubleClickData.clickIdList[0] = leftClickID;
		doubleClickData.lastTime = glfwGetTime();
		output = false;
	}
	else if (doubleClickData.clickCount == 1)
	{
		if (glfwGetTime() - doubleClickData.lastTime < 1.0f)
		{
			doubleClickData.clickIdList[1] = leftClickID;
			if (doubleClickData.clickIdList[1] == doubleClickData.clickIdList[0])
			{
				output = true;
			}
			else
			{
				doubleClickData.clickIdList[0] = leftClickID;
			}
		}
	}
	return output;
}

Picking::~Picking(){}

#pragma endregion

#pragma region PickingUi Class

/* static variables */
unsigned int PickingUI::pickingID_UI = PICKING_ID_MAX;
map<unsigned int, string> PickingUI::pickingList_UI;

unsigned int PickingUI::obtainPickingID(void)
{
	unsigned int pickingID = pickingID_UI;
	pickingID_UI--;
	return pickingID;
}

unsigned int PickingUI::getLastID(void)
{
	return pickingID_UI;
}

void PickingUI::resetPicking(void)
{
	pickingID_UI = PICKING_ID_MAX;
	pickingList_UI.clear();
	pickingList_UI[0] = "background";
}

void PickingUI::addValueToPickingList(unsigned int picking_id, const string par_value)
{
	pickingList_UI[picking_id] = par_value;
}

string PickingUI::getPickedObjectName(const unsigned int par_pickingID_UI)
{
	return pickingList_UI[par_pickingID_UI];
}

PickingUI::~PickingUI() {}

#pragma endregion

#pragma region PickingObject Class

/* static variables */
unsigned int PickingObject::pickingID_Object = PICKING_ID_MIN;
vector<unsigned int> PickingObject::unsedPickingID;

unsigned int PickingObject::GetPickingId(void)
{
	unsigned int pickingID = pickingID_Object;
	
	if (unsedPickingID.empty() == false)
	{
		pickingID = unsedPickingID[unsedPickingID.size() - 1];
		unsedPickingID.pop_back();
	}
	else
	{
		pickingID_Object += 1;
	}
	return pickingID;
}

void PickingObject::addUnsedPickingID(const unsigned int par_pickingID)
{
	unsedPickingID.push_back(par_pickingID);
}

unsigned int PickingObject::getLastPickingID(void)
{
	return pickingID_Object;
}

void PickingObject::resetPicking(void)
{
	pickingID_Object = PICKING_ID_MIN;
	unsedPickingID.clear();
}

PickingObject::~PickingObject(){}

#pragma endregion