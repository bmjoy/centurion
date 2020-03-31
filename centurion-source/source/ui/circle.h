#ifndef CIRCLE_H
#define CIRCLE_H

#include <gl_circle.h>

namespace gui {

	/// <summary>
	/// Class of Circle
	/// </summary>
	class Circle {
	public:

		/// <summary>
		/// Costructor of circle
		/// </summary>
		Circle();

		/// <summary>
		/// This function create a circle
		/// </summary>
		/// <param name="x">X postion of circle</param>
		/// <param name="y">y position of circle</param>
		/// <param name="w">width of the circle</param>
		/// <param name="h">height of the circle</param>
		/// <param name="borderWidth">bordedr dimension of the circle</param>
		/// <param name="origin">origin of the circle</param>
		void create(float x, float y, float w, float h, float borderWidth, std::string origin);

		/// <summary>
		/// This function render the circle
		/// </summary>
		/// <param name="Color">Color of the circle</param>
		/// <param name="x">x position of the rendered circle</param>
		/// <param name="y">y position of the rendered cirlce</param>
		/// <param name="w">width of rendered circle</param>
		/// <param name="h">height of rendered circle</param>
		/// <param name="origin">origin of rendered cirlce</param>
		void render(glm::vec4 Color, float x = 0.f, float y = 0.f, float w = 0.f, float h = 0.f, int origin = -1);
		
		/// <summary>
		/// Destructor of Circle
		/// </summary>
		~Circle();
	private:

		/// <summary>
		/// Data of Circle
		/// </summary>
		glCircle::CircleData data;

		/// <summary>
		/// Map origin of the circle
		/// </summary>
		std::map<std::string, int> originMap;
	};
};

#endif