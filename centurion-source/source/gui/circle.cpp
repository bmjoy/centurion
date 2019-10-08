#include <gui>

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	Circle::Circle() {
		originMap["bottom-left"] = 0;
		originMap["top-left"] = 1;
		originMap["center"] = 2;
		originMap["top-right"] = 3;
		originMap["bottom-right"] = 4;
		data = CircleData();
		data.type = "filled";
	}

	void Circle::create(string Type, float x, float y, float w, float h, float borderWidth, string origin) {
		if (x < 0) x += getParam("window-width");
		if (y < 0) y += getParam("window-height");
		data.type = Type;
		data.x = x;
		data.y = y;
		data.w = w;
		data.h = h;
		data.origin = originMap[origin];
		data.borderWidth = borderWidth;
	}
	void Circle::render(vec4 Color, float x, float y, float w, float h, int origin) {
		if (x != 0.f) data.x = x;
		if (y != 0.f) data.y = y;
		if (w != 0.f) data.w = w;
		if (h != 0.f) data.h = h;
		if (origin != -1) data.origin = origin;
		if (data.type == "border") {
			data.borderColor = Color;
			obj::ECircle()->render(data);
		}
	}

	Circle::~Circle(){}
}