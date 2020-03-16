#ifndef __PICKING_H
#define __PICKING_H

#define PICKING_ID_MIN 1
#define PICKING_ID_MAX 16777215
#define PICKING_LEFT 1
#define PICKING_RIGHT 2

#include <header.h>

using namespace glm;
using namespace std;

class Picking
{
public: 
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

	static unsigned int GetIdFromClick(const unsigned short int LeftRight = PICKING_LEFT);
	static vec3 getPickingColorFromID(const unsigned int par_pickingID);
	static void resetDoubleClickTime(void);
	static bool hasDoubleClicked(void);
	~Picking();

private:
	Picking();
};

class PickingUI : public Picking
{
public:
	static unsigned int obtainPickingID(void);
	static unsigned int getLastID(void);
	static void resetPicking(void);
	static void addValueToPickingList(unsigned int picking_id, const string par_value);
	static string getPickedObjectName(const unsigned int par_pickingID_UI);
	~PickingUI();
private:
	static unsigned int pickingID_UI;
	static map<unsigned int, string> pickingList_UI;
	PickingUI();
};

class PickingObject : public Picking
{
public:
	static unsigned int GetPickingId(void);
	static void addUnsedPickingID(const unsigned int par_pickingID);
	static unsigned int getLastPickingID(void);
	static void resetPicking(void);
	~PickingObject();
private:
	static unsigned int pickingID_Object;
	static vector<unsigned int> unsedPickingID;
	PickingObject();
};

#endif