#pragma once

#define MAX_NUMBER_OF_OBJECTS 100
#define MAX_NUMBER_OF_PLAYERS 10
#define TOWNHALL_RADIUS 1875
#define OUTPOST_RADIUS 700
#define MEDIUM_MAP_WIDTH 30000
#define MEDIUM_MAP_HEIGHT 20000

#include <headers>
#include <gui>

class GObject;
class Building;

class Game {

public:
	class Surface {
	public:
		static bool IsGridEnabled() { return isGridEnabled; }
		static void EnableGrid() { isGridEnabled = true; }
		static void DisableGrid() { isGridEnabled = false; }
		static bool Wireframe;
		static void Reset();
		static void CreateNoise();
		static void UpdateGrid();
		static void Render(bool tracing);
		~Surface();
	private:
		Surface();
		static bool isGridEnabled;
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

	/*static void RemoveGameObject(int i) { GameObjects.erase(GameObjects.begin() + i); }
	static void AddGameObject(GObject o) { GameObjects.push_back(o); }
	static void ResetGameObjects() { GameObjects.clear(); }
	static std::vector<GObject> GameObjects;*/

	#pragma region GameObjects List and Methods

	static GObject* GameObjects[MAX_NUMBER_OF_OBJECTS];
	static void RemoveGameObject(int i);
	static void AddGameObject(int i, GObject* o) { GameObjects[i] = o; }
	static void ResetGameObjects();
	static GObject* GetGameObjectPtrById(int id) { return GameObjects[id]; }
	static int GetNumberOfGameObjects();
	static int GetNumberOfBuildings();
	static int GetNumberOfUnits();
	static int GetNumberOfDecorations();
	static bool IsGameObjectNotNull(int i);
	static vector<Building*> GetListOfIndipendentBuildings();

	#pragma endregion

	static bool IsCreated() { return isCreated; }
	static void SetNumberOfPlayers(int n) { numberOfPlayers = n; }
	~Game();

protected:
	Game();
	static void GenerateSettlements(vector<vec2> &locs);
	static void GenerateOutposts(vector<vec2> &locs);
	static void GoToPointFromMinimap();
	static void RenderObjectsPicking();
	static void RenderObjects();
	static glm::mat4 projectionMatrix, viewMatrix;
	static bool isCreated;
	static int numberOfPlayers;

private:
	static float cameraToX, cameraToY;
};
