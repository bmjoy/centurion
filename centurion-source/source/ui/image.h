#ifndef IMAGE_H
#define IMAGE_H

#include <image_sprite.h>

namespace gui {
	class Image {
	public:
		Image();
		Image(string ImageName);
		int ObtainPickingID() { return imgData.pickingID; }
		void create(string Origin, float x, float y, float w, float h, int pickingID);
		void render(bool picking, float x = 0.f, float y = 0.f, bool repeat = false);
		vec2 getImageSize() { return vec2(imgData.w, imgData.h); }
		~Image();
	private:
		ImageSprite::ImageData imgData;
		string imageName;
		map<string, int> originMap;
	};
};

#endif