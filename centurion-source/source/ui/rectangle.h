#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <gl_rectangle.h>

namespace gui {
	class Rectangle {
	public:
		Rectangle();
		void create(string type, float x, float y, float w, float h, string origin, int pickingID, string luaCMD = "");
		void render(vec4 Color, bool picking = false, int leftClickId = 0, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Rectangle();
	private:
		int pickingId;
		string luaCmd;
		glRectangle::RectangleData data;
		map<string, int> originMap;
	};
};

#endif