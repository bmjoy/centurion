#ifndef CIRCLE_H
#define CIRCLE_H

#include <gl_circle.h>

namespace gui {
	class Circle {
	public:
		Circle();
		void create(float x, float y, float w, float h, float borderWidth, std::string origin);
		void render(glm::vec4 Color, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Circle();
	private:
		glCircle::CircleData data;
		std::map<std::string, int> originMap;
	};
};

#endif