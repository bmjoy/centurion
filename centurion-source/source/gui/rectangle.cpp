#include "Rectangle.h"


namespace gui {
	Rectangle::Rectangle()
	{
		type = "filled";
		modelMat = glm::mat4(1.f);
		pickingColor = glm::vec4(0.f);
	}

	void Rectangle::create(std::string Type, float x, float y, float w, float h, std::string origin) {

		type = Type;
		if (type == "filled") {

			modelMat = glm::scale(glm::mat4(1.f), glm::vec3(w, h, 1.f));
			if (origin == "bottom-left") {
				modelMat = glm::translate(modelMat, glm::vec3(x / w, y / h, 0.0f));
			}
			else if (origin == "top-left") {
				modelMat = glm::translate(modelMat, glm::vec3(x / w, (y - h) / h, 0.0f));
			}
			else if (origin == "center") {
				modelMat = glm::translate(modelMat, glm::vec3((x - w / 2) / w, (y - h / 2) / h, 0.0f));
			}

			// always set id before create

			pickingColor.r = (picking_id & 0x000000FF) >> 0;
			pickingColor.g = (picking_id & 0x0000FF00) >> 8;
			pickingColor.b = (picking_id & 0x00FF0000) >> 16;
			pickingColor.a = 1.f;
		}
	}

	void Rectangle::render(bool picking) {
		if (type == "filled") {
			if (picking) {
				obj::FRectangle()->render(modelMat, pickingColor);
			}
			else {
				obj::FRectangle()->render(modelMat, color);
			}
		}
	}

	Rectangle::~Rectangle()
	{
	}
}
