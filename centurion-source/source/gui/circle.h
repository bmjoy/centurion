#pragma once

#include <global.hpp>
#include <objects>

namespace gui {
	class Circle
	{
	public:
		Circle();
		void create(std::string type, float x, float y, float w, float h, float borderWidth, std::string origin);
		void render(glm::vec4 Color, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		~Circle();
	private:
		CircleData data;
		std::map<std::string, int> originMap;
		
	};
}
