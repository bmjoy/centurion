#include "Rectangle.h"

using namespace glb;

namespace gui {
	Rectangle::Rectangle()
	{
		originMap["bottom-left"] = 0;
		originMap["top-left"] = 1;
		originMap["center"] = 2;
		originMap["top-right"] = 3;
		originMap["bottom-right"] = 4;
		data = RectangleData();
		data.type = "filled";
	}

	void Rectangle::create(std::string Type, float x, float y, float w, float h, std::string origin, int pickingID) {

		if (x < 0) x += getParam("window-width");
		if (y < 0) y += getParam("window-height");

		data.type = Type;
		data.x = x;
		data.y = y;
		data.w = w;
		data.h = h;
		data.origin = originMap[origin];
		data.pickingColor = glm::vec4(getPickingColorFromID(pickingID), 1.f);
	}

	void Rectangle::render(glm::vec4 Color, bool picking, float x, float y, float w, float h, int origin) {

		if (x != 0.001f) data.x = x;
		if (y != 0.001f) data.y = y;
		if (w != 0.001f) data.w = w;
		if (h != 0.001f) data.h = h;
		if (origin != -1) data.origin = origin;

		if (data.type == "filled") {
			data.backColor = Color;
			obj::FRectangle()->render(data, picking);
		}
		if (data.type == "border") {
			data.borderColor = Color;
			obj::ERectangle()->render(data);
		}
		if (data.type == "border-filled") {
			data.backColor = Color;
			data.borderColor = glm::vec4(255.f);
			obj::FRectangle()->render(data, picking);
			obj::ERectangle()->render(data);
		}
	}

	Rectangle::~Rectangle()
	{
	}
}
