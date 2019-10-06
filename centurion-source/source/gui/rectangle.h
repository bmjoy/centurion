#pragma once

#include <global.hpp>
#include <objects.h>



namespace gui {
	class Rectangle
	{
	public:
		Rectangle();
		void create(std::string type, float x, float y, float w, float h, std::string origin, int pickingID);
		void render(glm::vec4 Color, bool picking = false, float x = 0.001f, float y = 0.001f, float w = 0.001f, float h = 0.001f, int origin = -1);
		~Rectangle();
	private:
		RectangleData data;
		std::map<std::string, int> originMap;
		
	};
}
