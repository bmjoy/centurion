#include <picking.h>
#include <global>
#include <engine/mouse.h>

unsigned int Picking::leftClickID = 0;
unsigned int Picking::leftClickID_UI = 0;
unsigned int Picking::rightClickID = 0;


/* PICKING */
unsigned int Picking::GetIdFromClick(const unsigned short int LeftRight)
{
	unsigned char data[4];
	//Edit the following line because you can get id with both left and right click
	if (LeftRight == Picking::PICKING_LEFT)
		glReadPixels((GLint)engine::Mouse::GetXLeftClick(), (GLint)engine::Mouse::GetYLeftClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
	if (LeftRight == Picking::PICKING_RIGHT)
		glReadPixels((GLint)engine::Mouse::GetXRightClick(), (GLint)engine::Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
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
	dCD.lastTime = glfwGetTime();
}

bool Picking::hasDoubleClicked(void)
{
	bool output = false;

	if (glfwGetTime() - dCD.lastTime > 1.0f)
	{
		dCD.clickCount = 0;
	}
	if (dCD.clickCount == 0)
	{
		dCD.clickCount++;
		dCD.clickIdList[0] = leftClickID;
		dCD.lastTime = glfwGetTime();
		output = false;
	}
	else if (dCD.clickCount == 1)
	{
		if (glfwGetTime() - dCD.lastTime < 1.0f)
		{
			dCD.clickIdList[1] = leftClickID;
			if (dCD.clickIdList[1] == dCD.clickIdList[0])
			{
				output = true;
			}
			else
			{
				dCD.clickIdList[0] = leftClickID;
			}
		}
	}
	return output;
}

/* PICKING_UI */
unsigned int PickingUI::getPickingID(void)
{
	unsigned int pickingID = this->pickingID_UI;
	this->pickingID_UI -= 1;

	return pickingID;
}

void PickingUI::resetPicking(void)
{
	this->pickingID_UI = PICKING_ID_MAX;
	this->pickingList_UI.clear();
	this->pickingList_UI[0] = "background";
}

void PickingUI::addValueToPickingList(const string par_value)
{
	this->pickingList_UI[PickingUI::getPickingID()] = par_value;
}

string PickingUI::getPickedObjectName(const unsigned int par_pickingID_UI)
{
	return this->pickingList_UI[par_pickingID_UI];
}

/* PICKING_OBJECT */

PickingObject *PickingObject::instance;
PickingObject *PickingObject::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new PickingObject();
	}
	return instance;
}
//PickingObject pickerObject = PickingObject::GetInstance();

unsigned int PickingObject::getPickingID(void)
{
	unsigned int pickingID = this->pickingID_Object;
	
	if (unsedPickingID.empty() == false)
	{
		pickingID = this->unsedPickingID[this->unsedPickingID.size() - 1];
		this->unsedPickingID.pop_back();
	}
	else
	{
		this->pickingID_Object += 1;
	}
	return pickingID;
}

void PickingObject::addUnsedPickingID(const unsigned int par_pickingID)
{
	this->unsedPickingID.push_back(par_pickingID);
}

unsigned int PickingObject::getLastPickingID(void)
{
	return this->pickingID_Object;
}

void PickingObject::resetPicking(void)
{
	this->pickingID_Object = PICKING_ID_MIN;
	this->unsedPickingID.clear();
}

/*
using namespace std;
using namespace glm;

namespace glb {
	int leftClickID = 0;
	int leftClickID_UI = 0;
	int rightClickID = 0;

	vector<unsigned int> unsedPickingID;

	void increasePickingID()
	{
		pickingID++;
	}
	void increasePickingID_UI()
	{
		pickingID_UI--;
	}
	unsigned int getPickingID()
	{
		unsigned int var_pickingID = pickingID;
		if (unsedPickingID.empty() == false)
		{
			var_pickingID = unsedPickingID[unsedPickingID.size() - 1];
			unsedPickingID.pop_back();
		}
		return var_pickingID;
	}
	void addUnsedPickingID(const unsigned int par_pickingID)
	{
		unsedPickingID.push_back(par_pickingID);
	}
	int getPickingID_UI() 
	{ 
		return pickingID_UI; 
	}

	unsigned int getLastPickingID()
	{
		return pickingID;
	}

	void addValueToPickingListUI(int pick, string value) 
	{ 
		pickingListUI[pick] = value; 
	}
	string getPickedObjectName(int pick) 
	{ 
		return pickingListUI[pick]; 
	}
	void resetDoubleClickTime() 
	{ 
		dCD.lastTime = glfwGetTime(); 
	}
	void resetPicking() 
	{
		pickingID = PICKING_ID_MIN;
	}
	void resetPicking_UI() 
	{
		pickingID_UI = PICKING_ID_MAX;
		pickingListUI.clear();
		pickingListUI[0] = "background";
	}
	int get_id(string LeftRight)
	{
		unsigned char data[4];
		//Edit the following line because you can get id with both left and right click
		if (LeftRight == "left")
			glReadPixels((GLint)engine::Mouse::GetXLeftClick(), (GLint)engine::Mouse::GetYLeftClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		if (LeftRight == "right")
			glReadPixels((GLint)engine::Mouse::GetXRightClick(), (GLint)engine::Mouse::GetYRightClick(), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;
		return pickedID;
	}
	vec3 getPickingColorFromID(int pickingID)
	{
		unsigned int r = (pickingID & 0x000000FF) >> 0;
		unsigned int g = (pickingID & 0x0000FF00) >> 8;
		unsigned int b = (pickingID & 0x00FF0000) >> 16;
		return vec3(r, g, b);
	}
	bool hasDoubleClicked() 
	{
		bool output = false;

		if (glfwGetTime() - dCD.lastTime > 1.0f) 
		{ 
			dCD.clickCount = 0;
		}
		if (dCD.clickCount == 0) 
		{
			dCD.clickCount++;
			dCD.clickIdList[0] = leftClickID;
			dCD.lastTime = glfwGetTime();
			output = false;
		}
		else if (dCD.clickCount == 1) 
		{
			if (glfwGetTime() - dCD.lastTime < 1.0f)
			{
				dCD.clickIdList[1] = leftClickID;
				if (dCD.clickIdList[1] == dCD.clickIdList[0]) 
				{
					output = true;
				}
				else 
				{
					dCD.clickIdList[0] = leftClickID;
				}
			}
		}
		return output;
	}
};
*/
