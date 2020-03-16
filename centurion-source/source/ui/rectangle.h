#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <primitives.h>

namespace gui {
	class Rectangle {
	public:
		Rectangle();
		void create(string type, float x, float y, float w, float h, string origin, int pickingID);
		void render(vec4 Color, bool picking = false, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Rectangle();
	private:
		RectangleData data;
		map<string, int> originMap;
	};
};

#endif