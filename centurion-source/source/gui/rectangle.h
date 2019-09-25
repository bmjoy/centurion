#pragma once

#include <global.hpp>
#include <objects.h>
#include "ui_object.h"

namespace gui {
	class Rectangle : public UIObject
	{
	public:
		Rectangle();
		void create(std::string type, float x, float y, float w, float h, std::string origin);
		void render(bool picking = false);
		~Rectangle();
	private:
		glm::vec4 pickingColor;
		glm::mat4 modelMat;
		std::string type;
	};
}
