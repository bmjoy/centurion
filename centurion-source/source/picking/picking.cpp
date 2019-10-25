#include <picking>
#include <global>

using namespace std;
using namespace glm;

namespace glb {
	int leftClickID = 0;
	int leftClickID_UI = 0;
	int rightClickID = 0;

	void increasePickingID() { pickingID++; }
	int getPickingID() { return pickingID; }
	void addValueToPickingListUI(int pick, string value) { pickingListUI[pick] = value; }
	string getPickedObjectName(int pick) { return pickingListUI[pick]; }
	void resetDoubleClickTime() { dCD.lastTime = glfwGetTime(); }
	void resetPicking() {
		pickingID = 1;
		pickingListUI.clear();
		pickingListUI[0] = "background";
	}
	int get_id(string LeftRight) {
		unsigned char data[4];
		//Edit the following line because you can get id with both left and right click
		if (LeftRight == "left")
			glReadPixels((GLint)getParam("mouse-x-leftclick"), (GLint)getParam("mouse-y-leftclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		if (LeftRight == "right")
			glReadPixels((GLint)getParam("mouse-x-rightclick"), (GLint)getParam("mouse-y-rightclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;
		return pickedID;
	}
	vec3 getPickingColorFromID(int pickingID) {
		int r = (pickingID & 0x000000FF) >> 0;
		int g = (pickingID & 0x0000FF00) >> 8;
		int b = (pickingID & 0x00FF0000) >> 16;
		return vec3(r, g, b);
	}
	bool hasDoubleClicked() {
		bool output = false;

		if (glfwGetTime() - dCD.lastTime > 1.0f) { dCD.clickCount = 0; }
		if (dCD.clickCount == 0) {
			dCD.clickCount++;
			dCD.clickIdList[0] = leftClickID;
			dCD.lastTime = glfwGetTime();
			output = false;
		}
		else if (dCD.clickCount == 1) {
			if (glfwGetTime() - dCD.lastTime < 1.0f) {
				dCD.clickIdList[1] = leftClickID;
				if (dCD.clickIdList[1] == dCD.clickIdList[0]) {
					output = true;
				}
				else {
					dCD.clickIdList[0] = leftClickID;
				}
			}
		}
		return output;
	}
};