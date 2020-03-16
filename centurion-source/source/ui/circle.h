#ifndef CIRCLE_H
#define CIRCLE_H

#include <primitives.h>

namespace gui {
	class Circle {
	public:
		Circle();
		void create(string type, float x, float y, float w, float h, float borderWidth, string origin);
		void render(vec4 Color, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Circle();
	private:
		CircleData data;
		map<string, int> originMap;
	};
};

#endif