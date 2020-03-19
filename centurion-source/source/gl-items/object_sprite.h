#ifndef SPRITE_H
#define SPRITE_H

#include "shader.h"

class ObjectSprite : public Shader {

public:

	struct SpriteData {
		vec3 pickingColor;
		float width;
		float height;
		GLuint textureId;
		GLuint pickingId;
		int directions;
		int frames;
		bool isClickable;
	};

	ObjectSprite();
	void AddEntityPath(string Path) { entPathList.push_back(Path); }
	~ObjectSprite();

protected:

	vector<string> entPathList;
	unsigned char *data;
	unsigned int indices[6] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	GLfloat vertices[20] = {
		// positions				// uv coords		
		-0.5f,		-0.5f,		0.0f,		0.0f, 1.0,   // in basso a sx
		0.5f,		-0.5f,		0.0f,		1.0, 1.0,    // in basso a dx
		0.5f,		0.5f,		0.0f,		1.0, 0.0f,   // in alto a dx
		-0.5f,		0.5f,		0.0f,		0.0f, 0.0f   // in alto a sx
	};
	
	void GenerateBuffers();

};

#endif 