#include <gui>
#include <picking.h>

#include <engine.h>

#include "empty_rectangle.h"
#include "filled_rectangle.h"

namespace gui {

	using namespace std;
	using namespace glm;
	

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
	void Rectangle::create(string Type, float x, float y, float w, float h, string origin, int pickingID) {

		if (x < 0) x += Engine::myWindow::Width;
		if (y < 0) y += Engine::myWindow::Height;

		data.type = Type;
		data.x = x;
		data.y = y;
		data.w = w;
		data.h = h;
		data.origin = originMap[origin];
		data.pickingColor = vec4(Picking::getPickingColorFromID(pickingID), 1.f);
	}
	void Rectangle::render(vec4 Color, bool picking, float x, float y, float w, float h, int origin) {

		if (x != 0.f) data.x = x;
		if (y != 0.f) data.y = y;
		if (w != 0.f) data.w = w;
		if (h != 0.f) data.h = h;
		if (origin != -1) data.origin = origin;

		if (data.type == "filled") {
			data.backColor = Color;
			FRectangle()->render(data, picking);
		}
		if (data.type == "border") {
			data.borderColor = Color;
			ERectangle()->render(data);
		}
		if (data.type == "border-filled") {
			data.backColor = Color;
			data.borderColor = vec4(255.f);
			FRectangle()->render(data, picking);
			ERectangle()->render(data);
		}
	}

	Rectangle::~Rectangle() {}

};
