#include <gui>

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	Image::Image(){}

	Image::Image(string ImageName) {
		imgData = ImageData();
		imageName = ImageName;
		originMap["bottom-left"] = 0;
		originMap["center"] = 1;
		originMap["top-left"] = 2;
	}
	void Image::create(string Origin, float x, float y, float w, float h, int pickingID) {
		if (x < 0) x += getParam("window-width");
		if (y < 0) y += getParam("window-height");
		imgData.imageName = imageName;
		imgData.pickingColor = getPickingColorFromID(pickingID);
		imgData.origin = originMap[Origin];
		imgData.x = x;
		imgData.y = y;
		obj::Img()->getImageData(&imgData);
		if (w != 0) imgData.w = w;
		if (h != 0) imgData.h = h;
	}
	void Image::render(bool picking, float x, float y) {
		if (x != 0.f) imgData.x = x;
		if (y != 0.f) imgData.y = y;
		obj::Img()->render(imgData, picking);
	}

	Image::~Image()	{}
}

