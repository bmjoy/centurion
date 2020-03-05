#pragma once

#include <headers>
#include <gui>

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

	
	class SelectionRectangle {
	public:
		struct SelRectPoints {
			float startX, startY, lastX, lastY;
			float minX, maxX, minY, maxY;
		};
		static SelRectPoints Coordinates;

		static bool IsInRectangle(std::array<float, 8> &coords);
		static void Create();
		static void Render();
		static float cameraLastX, cameraLastY;


	private:
		SelectionRectangle();
		static gui::Rectangle selRectangle;
	};


	class Minimap {
	public:
		static void Create();
		static void Render();
		static void Update() { IsCreated = false; }
		static bool IsCreated;
		~Minimap();
	private:
		Minimap();
	};



	Game();
	virtual void Create() {};
	virtual void Run() {};
	~Game();

protected:
	static void RenderObjectsPicking();
	static void RenderObjects();
};

