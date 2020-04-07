#include <picking.h>

#include <engine.h>
#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace Picking 
{
	struct DoubleClickData
	{
		double lastTime = 0;
		int clickCount = 0;
		int clickIdList[2] = { 0, 0 };
	};

	struct PickingData
	{
		unsigned int leftclickID;
		unsigned int rightclickID;
		unsigned int pickingID;
	};

	namespace
	{
		unsigned int rightClickID = 0;
		DoubleClickData doubleClickData;
	};

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

	vec3 Picking::GetPickingColorFromID(const unsigned int par_pickingID)
	{
		unsigned int r = (par_pickingID & 0x000000FF) >> 0;
		unsigned int g = (par_pickingID & 0x0000FF00) >> 8;
		unsigned int b = (par_pickingID & 0x00FF0000) >> 16;
		return vec3(r, g, b);
	}

	void Picking::ResetDoubleClickTime(void)
	{
		doubleClickData.lastTime = glfwGetTime();
	}

	bool Picking::HasDoubleClicked(void)
	{
		bool output = false;

		if (glfwGetTime() - doubleClickData.lastTime > 1.0f)
		{
			doubleClickData.clickCount = 0;
		}
		if (doubleClickData.clickCount == 0)
		{
			doubleClickData.clickCount++;
			doubleClickData.clickIdList[0] = Obj::GetLeftClickId();
			doubleClickData.lastTime = glfwGetTime();
			output = false;
		}
		else if (doubleClickData.clickCount == 1)
		{
			if (glfwGetTime() - doubleClickData.lastTime < 1.0f)
			{
				doubleClickData.clickIdList[1] = Obj::GetLeftClickId();
				if (doubleClickData.clickIdList[1] == doubleClickData.clickIdList[0])
				{
					output = true; //Ho fatto doppio click
				}
				else
				{
					doubleClickData.clickIdList[0] = Obj::GetLeftClickId();
				}
			}
		}
		return output;
	}


	namespace UI 
	{

		namespace 
		{
			Picking::PickingData data = PickingData();
		};

		void Picking::UI::ResetClickIds(void)
		{
			data.leftclickID = 0;
			data.rightclickID = 0;
		}

		unsigned int Picking::UI::GetLeftClickId(void)
		{
			return data.leftclickID;
		}

		unsigned int Picking::UI::GetRightClickId(void)
		{
			return data.rightclickID;
		}

		void Picking::UI::UpdateClickIds(void)
		{
			if (Engine::Mouse::LeftClick)
			{
				data.leftclickID = GetIdFromClick(PICKING_LEFT);
			}
			else if (Engine::Mouse::RightClick)
			{
				data.rightclickID = GetIdFromClick(PICKING_RIGHT);
			}
		}

		unsigned int Picking::UI::ObtainPickingID(void)
		{
			unsigned int newPickingID = data.pickingID;
			data.pickingID--;
			return newPickingID;
		}

		unsigned int Picking::UI::GetLastPickingID(void)
		{
			return data.pickingID;
		}

		void Picking::UI::ResetPicking(void)
		{
			data.pickingID = PICKING_ID_MAX;
		}

	};

	namespace Obj 
	{
		namespace
		{
			Picking::PickingData data = PickingData();
			vector<unsigned int> unsedPickingID = vector<unsigned int>();
		};

		void Picking::Obj::ResetClickIds(void)
		{
			data.leftclickID = 0;
			data.rightclickID = 0;
		}

		unsigned int Picking::Obj::GetLeftClickId(void)
		{
			return data.leftclickID;
		}

		unsigned int Picking::Obj::GetRightClickId(void)
		{
			return data.rightclickID;
		}

		void Picking::Obj::UpdateClickIds(void)
		{
			if (Engine::Mouse::LeftClick)
			{
				data.leftclickID = GetIdFromClick(PICKING_LEFT);
			}
			else if (Engine::Mouse::RightClick)
			{
				data.rightclickID = GetIdFromClick(PICKING_RIGHT);
			}
		}

		unsigned int Picking::Obj::ObtainPickingID(void)
		{
			unsigned int pickingID = data.pickingID;

			if (unsedPickingID.empty() == false)
			{
				pickingID = unsedPickingID[unsedPickingID.size() - 1];
				unsedPickingID.pop_back();
			}
			else
			{
				data.pickingID += 1;
			}
			return pickingID;
		}

		void Picking::Obj::AddUnsedPickingID(const unsigned int par_pickingID)
		{
			unsedPickingID.push_back(par_pickingID);
		}

		unsigned int Picking::Obj::GetLastPickingID(void)
		{
			return data.pickingID;
		}

		void Picking::Obj::ResetPicking(void)
		{
			data.pickingID = PICKING_ID_MIN;
			unsedPickingID.clear();
		}

	};


};


