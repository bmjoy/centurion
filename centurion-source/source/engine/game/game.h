#pragma once

#define MAX_NUMBER_OF_OBJECTS 100
#define MAX_NUMBER_OF_PLAYERS 8
#define TOWNHALL_RADIUS 1875
#define OUTPOST_RADIUS 700
#define MEDIUM_MAP_WIDTH 30000
#define MEDIUM_MAP_HEIGHT 20000

#include <ui.h>

class GObject;
class Building;
class Decoration;
class Unit;
//class PickingObject;

class Game {

public:
	class Map {
	public:

		static void LoadScenario(string scenarioName);
		static void SaveScenario(string scenarioName);

		static bool IsGridEnabled() { return isGridEnabled; }
		static void EnableGrid() { isGridEnabled = true; }
		static void DisableGrid() { isGridEnabled = false; }
		static bool Wireframe;
		static void Reset();
		static void CreateNoise();
		static void UpdateGrid();
		static void Render(bool tracing);
		~Map();
	private:
		Map();
		static void SaveMapObjectsToXml(string xmlPath);
		static void SaveHeights(string path);
		static void SaveTexture(string path);
		static void LoadMapObjectsFromXml(string xmlPath);
		static void LoadHeights(string path);
		static void LoadTexture(string path);
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

	class Race {
	public:
		Race();
		void setRaceProperties(int p1, string p2, string p3, string p4) { p_id = p1; name = p2; zone = p3; t_class = p4; }
		int getRaceId() { return p_id; };
		string getRaceName() { return name; }
		string getEnvironmentalZone() { return zone; }
		string getFoodTransportClass() { return t_class; }
		~Race();
	private:
		int p_id;
		string name;
		string zone;
		string t_class;
	};

#pragma region TO-LUA Methods

	static GObject* GetSelectedObject() { return selectedObject; }
	static bool IsGameObjectSelected(int id);
	static bool IsGameObjectNotNull(int id);
	static void CreateObject(string className, float x, float y, int player);
#pragma endregion


#pragma region GameObjects List and Methods

	static GObject* GameObjects[MAX_NUMBER_OF_OBJECTS]; // protected?
	static void RemoveGameObject(int i);
	static void AddGameObject(int i, GObject* o) { GameObjects[i] = o; }
	static void ResetGameObjects();
	static GObject* GetGameObjectPtrById(int id) { return GameObjects[id]; }
	static int GetNumberOfGameObjects();
	static int GetNumberOfBuildings();
	static int GetNumberOfUnits();
	static int GetNumberOfDecorations();
	static vector<Building*> GetListOfIndipendentBuildings();
	static vector<Building*> GetListOfStandAloneBuildings();
	static vector<Building*> GetListOfBuildings();
	static vector<Unit*> GetListOfUnits();
	static vector<Decoration*> GetListOfDecorations();
	static void UpdateSettlementBuildings();
	static void SetSelectedObject(GObject* o) { selectedObject = o; }

#pragma endregion

#pragma region Race methods

	static void AddRace(string race_name, Race r);
	static vector<string> GetListOfRacesNames() { return racesName; }
	static Race* GetRace(string race_name);

#pragma endregion

#pragma region Colors methods

	static void AddColor(vec3 col) { listOfColors.push_back(col); }
	static vector<vec3> GetListOfColors() { return listOfColors; }
	static vec3 GetColor(int i) { return listOfColors[i]; }
	static int GetNumberOfColors() { return (int)listOfColors.size(); }

#pragma endregion

	static bool IsCreated() { return isCreated; }
	static void SetNumberOfPlayers(int n) { numberOfPlayers = n; }
	~Game();

protected:
	Game();
	static glm::mat4 projectionMatrix, viewMatrix;
	static bool isCreated;
	static int numberOfPlayers;
	static vector<string> racesName;
	static map<string, Race> races;
	static vector<vec3> listOfColors;


	static void GenerateSettlements(vector<vec2> &locs);
	static void GenerateOutposts(vector<vec2> &locs);
	static void GoToPointFromMinimap();
	static void RenderObjectsPicking();
	static void RenderObjects();

private:
	static float cameraToX, cameraToY;
	static GObject* selectedObject;
};
