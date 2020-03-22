/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GLRECTANGLE_H
#define GLRECTANGLE_H

#include "shader.h"

/// <summary>
/// This class is used to render empty (or border) and filled rectangles.
/// </summary>
class glRectangle : public Shader {
public:

	/// <summary>
	/// This struct holds the rectangle information
	/// </summary>
	struct RectangleData {
		vec4 borderColor, backColor, pickingColor;
		string type;
		float x, y, w, h;
		int origin;
	};

	/// <summary>
	/// Constructor
	/// </summary>
	glRectangle() {}

	/// <summary>
	/// Create function
	/// </summary>
	virtual void create() {}

	/// <summary>
	/// Render function 1
	/// </summary>
	/// <param name="data">The RectangleData; you have to fill all the fields to properly render the rectangle;</param>
	/// <param name="picking">Boolean; true = picking is active;</param>
	virtual void render(RectangleData &data, bool picking = false) {}

	/// <summary>
	/// Render function 2
	/// </summary>
	/// <param name="data">The RectangleData; you have to fill all the fields to properly render the rectangle;</param>
	virtual void render(RectangleData &data) {}

	/// <summary>
	/// Destructor
	/// </summary>
	~glRectangle() {}

protected:
	float vertices[24];
};

/// <summary>
/// This class contains methods for empty rectangle
/// </summary>
class EmptyRectangle : public glRectangle
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	EmptyRectangle();

	/// <summary>
	/// Create function, override
	/// </summary>
	void create() override;

	/// <summary>
	/// Render function, override
	/// </summary>
	/// <param name="data"></param>
	void render(RectangleData &data) override;

	/// <summary>
	/// Destructor	
	/// </summary>
	~EmptyRectangle();	
};

/// <summary>
/// This class contains methods for filled rectangle
/// </summary>
class FilledRectangle : public glRectangle
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	FilledRectangle();

	/// <summary>
	/// Create function, override
	/// </summary>
	void create() override;

	/// <summary>
	/// Render function, override
	/// </summary>
	void render(RectangleData &data, bool picking = false) override;

	/// <summary>
	/// Destructor
	/// </summary>
	~FilledRectangle();
};

static EmptyRectangle eRect;
static FilledRectangle fRect;

#endif