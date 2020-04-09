/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GL_GRID_H
#define GL_GRID_H

#include "gl_shader.h"

/// <summary>
/// This class is used to render the PASS grid.
/// </summary>

class glGrid : public glShader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	glGrid();

	/// <summary>
	/// This function creates an empty grid texture based on the size of the pass grid;
	/// </summary>
	void create();

	/// <summary>
	/// This function updates the grid taking the data from astar::GridMatrix2D();
	/// </summary>
	void update();

	/// <summary>
	/// Render function;
	/// </summary>
	void render();

	/// <summary>
	/// Destructor
	/// </summary>
	~glGrid();

private:
	GLuint textureID;
};

#endif