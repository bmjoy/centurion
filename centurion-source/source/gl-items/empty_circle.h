/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef EMPTYCIRCLE_H
#define EMPTYCIRCLE_H

#include "shader.h"

/// <summary>
/// This class is used to render empty circles (or border circles).
/// </summary>
class EmptyCircle : public Shader
{
public:
	/// <summary>
	/// This struct holds the circle data needed to use the render function
	/// </summary>
	struct CircleData {
		vec4 borderColor, backColor;
		float x, y, w, h;
		float borderWidth;
		int origin;
	};

	/// <summary>
	/// Constructor
	/// </summary>
	EmptyCircle();

	/// <summary>
	/// Create function
	/// </summary>
	void create();

	/// <summary>
	/// Render function.
	/// </summary>
	/// <param name="data">The Circle data; you have to fill all the fields to properly render the circle.</param>
	void render(CircleData &data);

	/// <summary>
	/// Destructor
	/// </summary>
	~EmptyCircle();
};

static EmptyCircle eCircle;

#endif