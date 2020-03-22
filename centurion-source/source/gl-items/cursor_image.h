#ifndef CURSORIMAGE_H
#define CURSORIMAGE_H

#ifndef CURSOR_DEFAULT
#define CURSOR_DEFAULT 0
#endif // !CURSOR_DEFAULT

#ifndef CURSOR_RIGHT_CLICK
#define CURSOR_RIGHT_CLICK 1
#endif // !CURSOR_RIGHT_CLICK

#ifndef CURSOR_LEFT_CLICK
#define CURSOR_LEFT_CLICK 2
#endif // !CURSOR_RIGHT_CLICK

#include "shader.h"

/// <summary>
/// This class contains the methods and properties used to create and render the mouse cursor.
/// </summary>
class CursorImage : public Shader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	CursorImage();

	/// <summary>
	/// Create function
	/// </summary>
	void create();

	/// <summary>
	/// Render function
	/// </summary>
	/// <param name="x">The mouse X-position;</param>
	/// <param name="y">The mouse Y-position;</param>
	/// <param name="currentState">The mouse currentstate;</param>
	void render(float x, float y, int currentState);

	/// <summary>
	/// Destructor
	/// </summary>
	~CursorImage();
private:
	float cursorImgWidth, cursorImgHeight;
	string initPath;
	array<string, 3> texturePaths;
	GLuint textureIds[3];
};

static CursorImage cursor;

#endif