#include <ui.h>

#include <hector-lua.h>

namespace gui {

	using namespace std;
	using namespace glm;

	Button::Button() {
		pickingId = 0;
	}

	void Button::create(string imageName, string Text, int x, int y, int pickingID, vec4 textColor, string luaCMD) {
		buttonImage = new Image(imageName);
		buttonImage->create("center", (float)x, (float)y, 0, 0, pickingID);
		buttonText = new SimpleText("static");
		buttonText->create_static(Text, "tahoma_13px", (float)x, (float)y, "center", "middle", textColor, "bold");

		// save data
		luaCmd = luaCMD;
		pickingId = pickingID;
	}


	void Button::render(bool picking, int leftClickId) {
		buttonImage->render(picking);
		if (!picking) {
			buttonText->render_static();

			if (pickingId == leftClickId) {
				Hector::ExecuteCommand(luaCmd);
			}
		}
	}

	Button::~Button() {	}
};