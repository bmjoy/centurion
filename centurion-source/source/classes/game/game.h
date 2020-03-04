#pragma once

#include <headers>

class Game {

public:
	class Surface {
	public:
		static bool Wireframe;
		static void Reset();
		static void CreateNoise();
		static void UpdateGrid();
		static void Render(bool tracing);
		~Surface();
	private:
		Surface();
	};

protected:
	static void RenderObjectsPicking();

};

