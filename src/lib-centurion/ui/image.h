/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef IMAGE_H
#define IMAGE_H

#include <gl_image_sprite.h>

namespace gui {

	/// <summary>
	/// This class handles UI images.
	/// </summary>
	class Image {
	public:
		Image();
		Image(std::string ImageName);

		/// <summary>
		/// This integer function returns the image picking ID.
		/// </summary>
		int ObtainPickingID() { return imgData.pickingID; }

		/// <summary>
		/// This function creates an instance of Image.
		/// </summary>
		/// <param name="Origin">Origin of the image</param>
		/// <param name="x">X postion of the image</param>
		/// <param name="y">Y position of the image</param>
		/// <param name="w">Width of the image</param>
		/// <param name="h">Height of the image</param>
		/// <param name="pickingID">This parameter sets the ID used during render picking.</param>
		void create(std::string Origin, float x, float y, float w, float h, int pickingID);

		/// <summary>
		/// This function starts the rendering process of the Image.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking.</param>
		/// <param name="x">X offset of the image</param>
		/// <param name="y">Y offset of the image</param>
		/// <param name="repeat">This boolean parameter enables or disables image repeating. Default value is false.</param>
		void render(bool picking, float x = 0.f, float y = 0.f, bool repeat = false);

		/// <summary>
		/// This vector2 function returns the image size.
		/// </summary>
		glm::vec2 getImageSize() { return glm::vec2(imgData.w, imgData.h); }
		~Image();
	private:
		glImageSprite::ImageData imgData;
		std::string imageName;
		std::map<std::string, int> originMap;
	};
};

#endif