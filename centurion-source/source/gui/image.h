#pragma once

#include <global.hpp>
#include <objects.h>

#include "ui_object.h"

namespace gui {
	class Image : public UIObject
	{
	public:
		Image();
		Image(std::string ImageName);
		void create(std::string Origin, float x, float y, float w = 0.f, float h = 0.f);
		void render(bool picking, float x = 0.f, float y = 0.f);
		~Image();
	private:
		ImageData imgData;
		std::string imageName;
		std::map<std::string, int> originMap;
	};
}


