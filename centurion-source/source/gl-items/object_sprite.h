/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "shader.h"

class ObjectSprite : public Shader {

public:

	/// <summary>
	/// Struct that holds the Sprite Data.
	/// It's work in progress (WIP)
	/// Probably will be promoted to Class
	/// </summary>
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

	/// <summary>
	/// Constructor
	/// </summary>
	ObjectSprite();

	/// <summary>
	/// Create function. It reads all the information of the .eo.xml files (Entity Object) 
	/// and adds it to SpriteData variables. Then these variables are sent to 
	/// their relative ObjectXMLClassData using the classname as key.
	/// </summary>
	void Create();

	/// <summary>
	/// Render function. Empty, it's work in progress (WIP)
	/// </summary>
	void Render();

	/// <summary>
	/// This function adds the path to the list of paths, that is read in the "Create" function.
	/// </summary>
	/// <param name="Path">The Entity Object XML file path.</param>
	void AddEntityPath(string Path) { entPathList.push_back(Path); }

	/// <summary>
	/// Destructor
	/// </summary>
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

	/// <summary>
	/// This function generates the OpenGL buffers.
	/// </summary>
	void GenerateBuffers();

};

#endif 