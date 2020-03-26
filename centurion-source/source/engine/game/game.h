#pragma once

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

using namespace std;

class Game 
{
public:
	class Map 
	{
	public:
		/// <summary>
		/// This function loads a new scenario. It can throw an exception if an error occurs during the loading of the scenario.
		/// </summary>
		/// <param name="scenarioName">The name of the scanario (not the path!).</param>
		static void LoadScenario(const string scenarioName);
		/// <summary>
		/// This function saves a scenario. 
		/// </summary>
		/// <param name="scenarioName">The name of the scanario (not the path!).</param>
		static void SaveScenario(const string scenarioName);

		/// <summary>
		/// This function checks if the grid in the game map is enabled or not.
		/// </summary>
		/// <returns>True if it is enabled; false otherwise.</returns>
		static bool IsGridEnabled(void);
		/// <summary>
		/// This function ebables the grid in the game map.
		/// </summary>
		static void EnableGrid(void);
		/// <summary>
		/// This function resets the grid in the game map.
		/// </summary>
		static void DisableGrid(void);
		/// <summary>
		/// This function resets the game map.
		/// </summary>
		static void Reset(void);
		/// <summary>
		/// This function adds noise to the terrain of the game map.
		/// </summary>
		static void CreateNoise(void);
		/// <summary>
		/// This function updates the grid of the game map.
		/// </summary>
		static void UpdateGrid(void);
		/// <summary>
		/// This function performs the rendering of the game map.
		/// </summary>
		/// <param name="tracing">(???)</param>
		static void Render(const bool tracing);

		static bool Wireframe;
		/// <summary>
		/// The destructor.
		/// </summary>
		~Map(void);
	private:
		Map(void);
		/// <summary>
		/// (???) Forse da rivedere.
		/// This function saves al the objectes contained in the map on an XML file. 
		/// </summary>
		/// <param name="xmlPath">The path of the XML file.</param>
		static void SaveMapObjectsToXml(const string xmlPath);
		/// <summary>
		/// This function saves on a file the heights of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		static void SaveHeights(const string path);
		/// <summary>
		/// This function saves on a file the textures of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		static void SaveTexture(const string path);
		/// <summary>
		/// (???) Da rivedere.
		/// This function loads from a file all the objects of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		static void LoadMapObjectsFromXml(const string xmlPath);
		/// <summary>
		/// This function loads from a file the heights of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		static void LoadHeights(const string path);
		/// <summary>
		/// This function loads from a file the textures of the map. It can throw an exception if an error occurs.
		/// </summary>
		/// <param name="path">The path of the file.</param>
		static void LoadTexture(const string path);

		static bool isGridEnabled; 
	};

	/// <summary>
	/// This class allow the multiple selection using a rectangle.
	/// </summary>
	class SelectionRectangle
	{
	public:
		/// <summary>
		/// The coordinates of the rectangular selection.
		/// </summary>
		struct SelRectPoints 
		{
			float startX, startY, lastX, lastY;
			float minX, maxX, minY, maxY;
		};
		static SelRectPoints Coordinates;
		static float cameraLastX, cameraLastY;

		/// <summary>
		/// (???) Cosa fa?
		/// </summary>
		/// <param name="coords"></param>
		/// <returns></returns>
		static bool IsInRectangle(std::array<float, 8> &coords);
		/// <summary>
		/// This function creates a rectangula selection.
		/// </summary>
		static void Create(void);
		/// <summary>
		/// This function performs the rende
		/// </summary>
		static void Render(void);
		/// <summary>
		/// This function checks if the rectaungular selection is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		static bool IsActive(void);
		/// <summary>
		/// This function enables the rectangula selection.
		/// </summary>
		static void Enable(void);
		/// <summary>
		/// This function disables the rectangular selection.
		/// </summary>
		static void Disable(void);

	private:
		SelectionRectangle(void);
		static gui::Rectangle selRectangle;
		static bool isActive;
	};

	// prerendered minimap "image"
	class Minimap
	{
	public:
		/// <summary>
		/// This function create a new minimap.
		/// </summary>
		static void Create(void);
		/// <summary>
		/// This function performs the rendering of a minimap.
		/// </summary>
		static void Render(void);
		/// <summary>
		/// This functions updates the minimap.
		/// </summary>
		static void Update(void);
		/// <summary>
		/// This function checks if the minimap was created.
		/// </summary>
		/// <returns>True or not.</returns>
		static bool IsCreated(void);
		/// <summary>
		/// This function enables the minimap.
		/// </summary>
		static void Enable(void);
		/// <summary>
		/// This function disables the minimap.
		/// </summary>
		static void Disable(void);
		/// <summary>
		/// This function checks if the minimap is active or not.
		/// </summary>
		/// <returns>True if it is active; false otherwise.</returns>
		static bool IsActive(void);
		/// <summary>
		/// This function checks if the minimap is blocked or not.
		/// </summary>
		/// <returns>True if it's blocked; false otherwise.</returns>
		static bool IsBlocked(void);
		/// <summary>
		/// This function blocks the game minimap.
		/// </summary>
		static void Block(void);
		/// <summary>
		/// This function unblocks the game minimap.
		/// </summary>
		static void Unblock(void);
		~Minimap();
	private:
		Minimap();
		static bool isActive;
		static bool isCreated;
		static bool isBlocked;
	};

	/// <summary>
	/// (???) Classe da rivedere e forse da posizionare altrove perché deve poter interagire con Object.h.
	/// </summary>
	class Race 
	{
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
	/// <summary>
	/// (???) Metodi da rivedere/spostare?
	/// </summary>
	/// <returns></returns>
	static GObject* GetSelectedObject(void) { return selectedObject; }
	static bool IsGameObjectSelected(const unsigned int id);
	static bool IsGameObjectNotNull(int id);
	static bool CreateObject(const string className, const float x, const float y, const unsigned int player);

#pragma endregion


#pragma region GameObjects List and Methods

	/// <summary>
	/// (???) Da rivedere.
	/// This function returns a list of all the indipendent buildings in the game.
	/// </summary>
	/// <returns>A list of buildings.</returns>
	static vector<Building*> GetListOfIndipendentBuildings(void);
	//static void UpdateSettlementBuildings();
	static void SetSelectedObject(GObject* o) { selectedObject = o; }

#pragma endregion

#pragma region Race methods

	/// <summary>
	/// Metodi da rivedere. La gestione delle razze va rivista perché deve poter interagire anche con object.h.
	/// </summary>
	/// <param name="race_name"></param>
	/// <param name="r"></param>
	static void AddRace(string race_name, Race r);
	static vector<string> GetListOfRacesNames() { return racesName; }
	static Race* GetRace(string race_name);

#pragma endregion

#pragma region Colors methods

	/// <summary>
	/// (???) Da sistemare --> dovrebbe controllare che il colore non è già presente.
	/// This function add a new color.
	/// </summary>
	/// <param name="col">A new color.</param>
	static void AddColor(const vec3 col);
	/// <summary>
	/// This function returns all the available colors.
	/// </summary>
	/// <returns>A list of color.</returns>
	static vector<vec3> GetListOfColors(void);
	/// <summary>
	/// (???) Cosa succede se il colore non è presente nella lista?
	/// This function returns a specific color.
	/// </summary>
	/// <param name="i">The index of the color.</param>
	/// <returns>A color.</returns>
	static vec3 GetColor(const unsigned int i);
	/// <summary>
	/// This function return the number of available colors.
	/// </summary>
	/// <returns>The number of available colors.</returns>
	static unsigned int GetNumberOfColors(void);

#pragma endregion

	//(???) Tutti i metodi a seguire potrebbero essere da rivedere.

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
