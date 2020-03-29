#include <ui.h>
#include <picking.h>

#include <engine.h>

namespace gui {

	Image::Image(){}

	Image::Image(string ImageName) {
		imgData = ImageSprite::ImageData();
		imageName = ImageName;
		originMap["bottom-left"] = 0;
		originMap["center"] = 1;
		originMap["top-left"] = 2;
	}
	void Image::create(string Origin, float x, float y, float w, float h, int pickingID) {
		if (x < 0) x += Engine::myWindow::Width;
		if (y < 0) y += Engine::myWindow::Height;
		imgData.pickingID = pickingID;
		imgData.imageName = imageName;
		imgData.pickingColor = PickingUI::GetPickingColorFromID(pickingID);
		imgData.origin = originMap[Origin];
		imgData.x = x;
		imgData.y = y;
		Img()->getImageData(&imgData);
		if (w != 0) imgData.w = w;
		if (h != 0) imgData.h = h;
		imgData.ratioX = imgData.w / imgData.texW;
		imgData.ratioY = imgData.h / imgData.texH;
	}
	void Image::render(bool picking, float x, float y, bool repeat) {
		if (x != 0.f) imgData.x = x;
		if (y != 0.f) imgData.y = y;
		Img()->render(imgData, picking, repeat);
	}

	Image::~Image()	{}
};