/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef MINIMAPRECTANGLE_H
#define MINIMAPRECTANGLE_H

#include "shader.h"

/// <summary>
/// This class is used to pre-render the minimap when the game starts 
/// In the editor, it updates every time you add a building or decoration
/// </summary>
class MinimapRectangle : public Shader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	MinimapRectangle();

	/// <summary>
	/// Creates the texture of the minimap
	/// </summary>
	void create();

	/// <summary>
	/// Update the texture
	/// </summary>
	void update();

	/// <summary>
	/// Render the texture, like an image
	/// </summary>
	void render();

	/// <summary>
	/// Destructor
	/// </summary>
	~MinimapRectangle();

private:
	int textureWidth, textureHeight;
	GLuint textureID;
};

#endif