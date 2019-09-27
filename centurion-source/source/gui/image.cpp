#include "image.h"

namespace gui {
	Image::Image(){}

	Image::Image(std::string ImageName) {
		imgData = ImageData();
		imageName = ImageName;
		originMap["bottom-left"] = 0;
		originMap["center"] = 1;
		originMap["top-left"] = 2;
	}

	void Image::create(std::string Origin, float x, float y, float w, float h) {
		imgData.imageName = imageName;
		imgData.pickingColor = pickingColor;
		imgData.origin = originMap[Origin];

		imgData.x = x;
		imgData.y = y;

		obj::Img()->getImageData(&imgData);

		if (w != 0) {
			imgData.w = w;
		}
		if (h != 0) {
			imgData.h = h;
		}
	}

	void Image::render(bool picking, float x, float y) {
		if (x != 0.f) {
			imgData.x = x;
		}
		if (y != 0.f) {
			imgData.y = y;
		}

		obj::Img()->render(imgData, picking);
	}

	Image::~Image()	{}
}

