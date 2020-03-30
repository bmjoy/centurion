/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef IMAGESPRITE_H
#define IMAGESPRITE_H

#include "shader.h"

/// <summary>
/// This class is used to render any image to the screen.
/// </summary>
class ImageSprite : public Shader
{
public:
	/// <summary>
	/// This struct contains every image information and it's needed to render the image
	/// </summary>
	struct ImageData {
		glm::vec3 pickingColor;
		std::string imageName;
		float x, y, w, h;
		float texW, texH, ratioX, ratioY;
		int pickingID;
		int origin;
		GLuint textureID;
	};

	/// <summary>
	/// Constructor.
	/// </summary>
	ImageSprite();

	/// <summary>
	/// This method adds images paths to the list of paths. They will be used in the create function to 
	/// prepare the textures for opengl.
	/// </summary>
	/// <param name="imageName">The image name, useful to identify an image in the whole code;</param>
	/// <param name="path">The image folder path;</param>
	void addPath(std::string imageName, std::string path) { imagesPathMap[imageName] = path; }

	/// <summary>
	/// This function reads every image and prepare the textureID.
	/// </summary>
	void create();

	/// <summary>
	/// This function provides the image data
	/// </summary>
	/// <param name="d">Pointer to the variable which will contain the image data</param>
	void getImageData(ImageData *d);
	
	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="imageData">The struct of image data. It's important to fill each field to properly renderize the image</param>
	/// <param name="picking">Boolean; true = picking is active;</param>
	/// <param name="repeat">Boolean; true = the image will be repeated if smaller of the specified size, false = the image will be stretched</param>
	void render(ImageData &imageData, bool picking = false, bool repeat = false);

	/// <summary>
	/// Destructor
	/// </summary>
	~ImageSprite();

private:
	std::map<std::string, std::string> imagesPathMap;
	std::map<GLuint, int[2]> imageSize;

	unsigned char *data;
	unsigned int indices[6] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	GLfloat vertices[20] = {
		// positions			// uv coords		
		0.f,	0.f,	0.f,	0.f, 1.f,	// in basso a sx
		1.f,	0.f,	0.f,	1.f, 1.f,	// in basso a dx
		1.f,	1.f,	0.f,	1.f, 0.f,	// in alto a dx
		0.f,	1.f,	0.f,	0.f, 0.f	// in alto a sx
	};

	/// <summary>
	/// This function generates the OpenGL buffers.
	/// </summary>
	void GenerateBuffers();
};

static ImageSprite img;

#endif