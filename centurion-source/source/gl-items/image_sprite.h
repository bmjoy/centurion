#ifndef IMAGESPRITE_H
#define IMAGESPRITE_H

#include "shader.h"

class ImageSprite : public Shader
{
public:
	ImageSprite();
	void addPath(string imageName, string path) { imagesPathMap[imageName] = path; }
	void create();
	void getImageData(ImageData *d);
	void render(ImageData &imageData, bool picking = false, bool repeat = false);
	~ImageSprite();

private:
	map<string, string> imagesPathMap;
	map<GLuint, int[2]> imageSize;

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

	void genBuffers();
};

static ImageSprite img;

#endif