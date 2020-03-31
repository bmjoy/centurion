#ifndef IMAGE_H
#define IMAGE_H

#include <image_sprite.h>

namespace gui {

	/// <summary>
	/// Class of image
	/// </summary>
	class Image {
	public:
		Image();
		Image(std::string ImageName);
		int ObtainPickingID() { return imgData.pickingID; }
		void create(std::string Origin, float x, float y, float w, float h, int pickingID);
		void render(bool picking, float x = 0.f, float y = 0.f, bool repeat = false);
		glm::vec2 getImageSize() { return glm::vec2(imgData.w, imgData.h); }
		~Image();
	private:
		ImageSprite::ImageData imgData;
		std::string imageName;
		std::map<std::string, int> originMap;
	};
};

#endif