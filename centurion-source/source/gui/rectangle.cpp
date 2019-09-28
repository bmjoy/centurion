#include "Rectangle.h"


namespace gui {
	Rectangle::Rectangle()
	{
		originMap["bottom-left"] = 0;
		originMap["top-left"] = 1;
		originMap["center"] = 2;
		data = RectangleData();
		data.type = "filled";
	}

	void Rectangle::create(std::string Type, float x, float y, float w, float h, std::string origin, int pickingID) {

		if (x < 0) x += GLB::WINDOW_WIDTH;
		if (y < 0) y += GLB::WINDOW_HEIGHT;

		data.type = Type;
		data.x = x;
		data.y = y;
		data.w = w;
		data.h = h;
		data.origin = originMap[origin];
		data.pickingColor = glm::vec4(getPickingColorFromID(pickingID), 1.f);
	}

	void Rectangle::render(glm::vec4 Color, bool picking, float x, float y, float w, float h, int origin) {

		if (x != 0) data.x = x;
		if (y != 0) data.y = y;
		if (w != 0) data.w = w;
		if (h != 0) data.h = h;
		if (origin != -1) data.origin = origin;

		if (data.type == "filled") {
			data.backColor = Color;
			obj::FRectangle()->render(data, picking);
		}
		if (data.type == "border") {
			data.borderColor = Color;
			obj::ERectangle()->render(data);
		}
	}

	Rectangle::~Rectangle()
	{
	}
}
