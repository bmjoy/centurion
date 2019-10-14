#include <picking>
#include <global>

using namespace std;
using namespace glm;

namespace glb {
	void increasePickingID() { pickingID++; }
	int getPickingID() { return pickingID; }
	void addValueToPickingListUI(int pick, string value) { pickingListUI[pick] = value; }
	string getPickedObjectName(int pick) { return pickingListUI[pick]; }
	void resetPicking() {
		pickingID = 1;
		pickingListUI = { };
		pickingListUI[0] = "background";
	}
	int get_id() {
		unsigned char data[4];
		glReadPixels((GLint)getParam("mouse-x-leftclick"), (GLint)getParam("mouse-y-leftclick"), 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &data);
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
};