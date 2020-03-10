#ifndef __PICKING_H
#define __PICKING_H

#include <headers>

using namespace glm;
using namespace std;

class Picking
{
public: 
	const static unsigned short int PICKING_LEFT = 1;
	const static unsigned short int PICKING_RIGHT = 2;

	static unsigned int leftClickID;
	static unsigned int leftClickID_UI;
	static unsigned int rightClickID;
	struct doubleClickData 
	{
		double lastTime = 0;
		int clickCount = 0;
		int clickIdList[2] = { 0, 0 };
	};
	static doubleClickData dCD;

	//static unsigned int getPickingID(void) {};
	//virtual void resetPicking(void);
	unsigned int GetIdFromClick(const unsigned short int LeftRight = Picking::PICKING_LEFT);
	vec3 getPickingColorFromID(const unsigned int par_pickingID);
	void resetDoubleClickTime(void);
	bool hasDoubleClicked(void);
	~Picking();
protected:
	const unsigned int PICKING_ID_MIN = 1;
	const unsigned int PICKING_ID_MAX = 16777215;
private:
	Picking();
};

class PickingObject : public Picking
{
public:
	static unsigned int getPickingID(void);
	static void addUnsedPickingID(const unsigned int par_pickingID);
	static unsigned int getLastPickingID(void);
	static void resetPicking(void);
	static PickingObject *GetInstance();
	~PickingObject();
private:
	static unsigned int pickingID_Object = PICKING_ID_MIN;
	static vector<unsigned int> unsedPickingID;
	static PickingObject *instance;
	PickingObject();
};
//extern PickingObject pickerObject;
class PickingUI : public Picking
{
public:
	unsigned int getPickingID(void) override;
	void resetPicking(void) override;
	void addValueToPickingList(const string par_value);
	string getPickedObjectName(const unsigned int par_pickingID_UI);
	~PickingUI();
private:
	unsigned int pickingID_UI = PICKING_ID_MAX;
	map<unsigned int, string> pickingList_UI;
	PickingUI();
};

#endif