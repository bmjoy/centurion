/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef CIRCLE_H
#define CIRCLE_H

#include <gl_circle.h>

namespace gui {

	/// <summary>
	/// This class handles circles in game.
	/// </summary>
	class Circle {
	public:
		Circle();

		/// <summary>
		/// This function create a circle.
		/// </summary>
		/// <param name="x">X postion of the circle</param>
		/// <param name="y">Y position of the circle</param>
		/// <param name="w">Width of the circle</param>
		/// <param name="h">Height of the circle</param>
		/// <param name="borderWidth">Border thickness of the circle</param>
		/// <param name="origin">Origin of the circle</param>
		void create(float x, float y, float w, float h, float borderWidth, std::string origin);

		/// <summary>
		/// This function renders the circle.
		/// </summary>
		/// <param name="Color">Color of the circle. It supports only a glm::vec4 RGBA</param>
		/// <param name="x">X offset of the circle</param>
		/// <param name="y">Y offset of the circle</param>
		/// <param name="w">Width offset of the circle</param>
		/// <param name="h">Height offset of the circle</param>
		/// <param name="origin">Origin of the circle</param>
		void render(glm::vec4 Color, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);

		~Circle();
	private:
		glCircle::CircleData data;
		std::map<std::string, int> originMap;
	};
};

#endif