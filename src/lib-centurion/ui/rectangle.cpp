#include <ui.h>
#include <picking.h>

#include <engine.h>

#include "hector-lua.h"

using namespace std;
using namespace glm;

namespace gui {

	Rectangle::Rectangle()
	{
		originMap["bottom-left"] = BOTTOMLEFT_ORIGIN;
		originMap["top-left"] = TOPLEFT_ORIGIN;
		originMap["center"] = CENTER_ORIGIN;
		originMap["top-right"] = TOPRIGHT_ORIGIN;
		originMap["bottom-right"] = BOTTOMRIGHT_ORIGIN;
		data = glRectangle::RectangleData();
		data.type = FILLED_RECTANGLE;
		pickingId = 0;
		luaCmd = "";
		isHover = false;
	}
	void Rectangle::create(string Type, float x, float y, float w, float h, string origin, int pickingID, string luaCMD) {

		if (x < 0) x += Engine::myWindow::Width;
		if (y < 0) y += Engine::myWindow::Height;

		if (Type == "border") {
			data.type = BORDER_RECTANGLE;
		}
		else if (Type == "border-filled") {
			data.type = BORDER_FILLED_RECTANGLE;
		}
		else {
			data.type = FILLED_RECTANGLE;
		}
		
		data.x = x;
		data.y = y;
		data.w = w;
		data.h = h;
		data.origin = originMap[origin];
		data.pickingColor = vec4(Picking::GetPickingColorFromID(pickingID), 1.f);
		pickingId = pickingID;
		luaCmd = luaCMD;
	}

	bool Rectangle::IsClicked(int leftclickID)
	{
		return (pickingId == leftclickID);
	}

	void Rectangle::render(vec4 backColor, glm::vec4 borderColor, bool picking, int leftClickId , float x, float y, float w, float h, int origin) {
		
		isHover = false;

		if (picking == false) {
			if (IsClicked(leftClickId)) {
				Hector::ExecuteCommand(luaCmd);
			}
		}

		if (x != 0.f) data.x = x;
		if (y != 0.f) data.y = y;
		if (w != 0.f) data.w = w;
		if (h != 0.f) data.h = h;
		if (origin != DEFAULT_ORIGIN) data.origin = origin;

		if (data.type == FILLED_RECTANGLE) {
			data.backColor = backColor;
			GLItems::FRectangle()->render(data, picking);
		}
		if (data.type == BORDER_RECTANGLE) {
			data.borderColor = borderColor;
			GLItems::ERectangle()->render(data);
		}
		if (data.type == BORDER_FILLED_RECTANGLE) {
			data.backColor = backColor;
			data.borderColor = borderColor;
			GLItems::FRectangle()->render(data, picking);
			GLItems::ERectangle()->render(data);
		}

		
	}

	Rectangle::~Rectangle() {}

};
