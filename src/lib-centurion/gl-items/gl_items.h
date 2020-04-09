/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AudioManager;
class glBitmapFont;
class glBuildingSprite;
class CursorImage;
class glDecorationSprite;
class EmptyRectangle;
class glCircle;
class FilledRectangle;
class ImageSprite;
class glUnitSprite;
class Terrain;
class Grid;
class MinimapRectangle;

/// <summary>
/// This namespace contains all the pointers to primitives istances (that are static)
/// You can find the static instance in the .h of that class
/// </summary>
namespace GLItems {

	AudioManager *Audio();
	glBitmapFont *Text();
	glBuildingSprite *BSprite();
	CursorImage *Cursor();
	glDecorationSprite *DSprite();
	EmptyRectangle *ERectangle();
	glCircle *ECircle();
	FilledRectangle *FRectangle();
	ImageSprite *Img();
	glUnitSprite *USprite();
	Terrain *MapTerrain();
	Grid *MapGrid();
	MinimapRectangle *MMRectangle();

	/// <summary>
	/// This function sets the menu projection matrix
	/// </summary>
	void setMenuProjectionMatrix(glm::mat4 mat);

	/// <summary>
	/// This function sets the camera projection matrix
	/// </summary>
	void setCameraProjectionMatrix(glm::mat4 mat);
	
	/// <summary>
	/// This function sets the minimap projection matrix
	/// </summary>
	void setMinimapProjectionMatrix();

	/// <summary>
	/// This function returns the camera projection matrix
	/// </summary>
	glm::mat4 getCameraProjectionMatrix(void);

	/// <summary>
	/// This function returns the minimap projection matrix
	/// </summary>
	glm::mat4 getMinimapProjectionMatrix(void);

	/// <summary>
	/// This function applies all the menu matrices to the primitives (especially the UI ones)
	/// </summary>
	void applyMenuMatrices();

	/// <summary>
	/// This function applies the game matrices to the primitives
	/// </summary>
	void applyGameMatrices(glm::mat4 *proj, glm::mat4 *view);

	/// <summary>
	/// This function launches the constructors of each primitive
	/// </summary>
	void init();

	/// <summary>
	/// This function compiles all the primitives
	/// </summary>
	void compile();

	/// <summary>
	/// This function creates all the primitives
	/// </summary>	
	void create();
};

#endif