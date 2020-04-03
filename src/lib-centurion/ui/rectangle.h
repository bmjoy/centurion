/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <gl_rectangle.h>

namespace gui {

	/// <summary>
	/// This class handles a UI rectangle.
	/// </summary>
	class Rectangle {
	public:
		Rectangle();

		/// <summary>
		/// This function creates a rectangle.
		/// </summary>
		/// <param name="type">Type of rectangle</param>
		/// <param name="x">X position of the rectangle</param>
		/// <param name="y">Y Position of the rectangle</param>
		/// <param name="w">Weight of the rectangle</param>
		/// <param name="h">Height of the rectangle</param>
		/// <param name="origin">Origin of the rectangle</param>
		/// <param name="pickingID">Picking ID value for render picking</param>
		/// <param name="luaCMD">LUA command that should be executed when this rectangle is created</param>
		void create(std::string type, float x, float y, float w, float h, std::string origin, int pickingID, std::string luaCMD = "");

		/// <summary>
		/// This function renders the rectangle
		/// </summary>
		/// <param name="Color">Color of the rectangle. It only support glm::vec4 RGBA</param>
		/// <param name="picking">This boolean parameter enables or disables render picking.</param>
		/// <param name="leftClickId">Picking ID value for picking</param>
		/// <param name="x">X offset of the rectangle</param>
		/// <param name="y">Y offset of the rectangle</param>
		/// <param name="w">Weight offest of the rectangle</param>
		/// <param name="h">Height offset of the rectangle</param>
		/// <param name="origin">Origin of the rectangle</param>
		void render(glm::vec4 Color, bool picking = false, int leftClickId = 0, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);


		~Rectangle();
	private:
		int pickingId;
		std::string luaCmd;
		glRectangle::RectangleData data;
		std::map<std::string, int> originMap;
	};
};

#endif