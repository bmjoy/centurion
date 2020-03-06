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
		static float cameraLastX, cameraLastY;

		static bool IsInRectangle(std::array<float, 8> &coords);
		static void Create();
		static void Render();
		static bool IsActive() { return isActive; }
		static void Enable() { isActive = true; }
		static void Disable() { isActive = false; }

	private:
		SelectionRectangle();
		static gui::Rectangle selRectangle;
		static bool isActive;
	};

	// prerendered minimap "image"
	class Minimap {
	public:
		static void Create();
		static void Render();
		static void Update() { isCreated = false; }
		static bool IsCreated() { return isCreated; }
		static void Enable() { isActive = true; }
		static void Disable() { isActive = false; }
		static bool IsActive() { return isActive; }
		static bool IsBlocked() { return isBlocked; }
		static void Block() { isBlocked = true; }
		static void Unblock() { isBlocked = false; }
		~Minimap();
	private:
		Minimap();
		static bool isActive;
		static bool isCreated;
		static bool isBlocked;
	};



	Game();
	static bool IsCreated() { return isCreated; }
	~Game();

protected:
	static void GoToPointFromMinimap();
	static void RenderObjectsPicking();
	static void RenderObjects();
	static glm::mat4 projectionMatrix, viewMatrix;
	static bool isCreated;

private:
	static float cameraToX, cameraToY;
};
