#include <ui.h>
#include <engine.h>

using namespace std;
using namespace glm;

namespace gui {

	Circle::Circle() {
		originMap["bottom-left"] = 0;
		originMap["top-left"] = 1;
		originMap["center"] = 2;
		originMap["top-right"] = 3;
		originMap["bottom-right"] = 4;
		data = glCircle::CircleData();
	}

	void Circle::create(float x, float y, float w, float h, float borderWidth, string origin) {
		if (x < 0) x += Engine::myWindow::Width;
		if (y < 0) y += Engine::myWindow::Height;
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
		data.borderColor = Color;
		GLItems::ECircle()->render(data);
	}

	Circle::~Circle() {}
};
