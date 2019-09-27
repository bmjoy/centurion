#pragma once

#include <global.hpp>
#include <objects.h>

namespace gui {
	class Rectangle
	{
	public:
		Rectangle();
		void create(std::string type, float x, float y, float w, float h, std::string origin, int pickingID);
		void render(glm::vec4 Color, bool picking = false);
		~Rectangle();
	private:
		glm::vec4 pickingColor;
		glm::mat4 modelMat;
		std::string type;
	};
}
