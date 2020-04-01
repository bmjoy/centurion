#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <gl_rectangle.h>

namespace gui {
	class Rectangle {
	public:
		Rectangle();
		void create(std::string type, float x, float y, float w, float h, std::string origin, int pickingID, std::string luaCMD = "");
		void render(glm::vec4 Color, bool picking = false, int leftClickId = 0, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Rectangle();
	private:
		int pickingId;
		std::string luaCmd;
		glRectangle::RectangleData data;
		std::map<std::string, int> originMap;
	};
};

#endif