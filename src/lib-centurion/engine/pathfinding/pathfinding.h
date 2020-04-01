#pragma once

#include <header.h>

namespace astar {

	//
	//	These variables contain the GRID information to generate pathfinding
	//

	static const int cellGridSize = 20;
	static const int gridWidth = 1500;
	static const int gridHeight = 1000;
	static int gridMatrix[gridWidth * gridHeight] = { 0 };
	static int gridMatrix2D[gridWidth * gridHeight] = { 0 };
	static int closedNodes[gridWidth * gridHeight] = { 0 };
	static int openNodes[gridWidth * gridHeight] = { 0 };
	static int dirMap[gridWidth * gridHeight] = { 0 };

	//
	//	ASTAR FUNCTIONS --> source/pathfinding/astar.cpp
	// 
	
	struct Location {
		int row, col;
		Location() {
			row = col = 0;
		};
		Location(int r, int c) {
			row = r;
			col = c;
		};
	};

	int *GridMatrix();
	int *GridMatrix2D();
	int *ClosedNodes();
	int *OpenNodes();
	int *DirMap();
	std::vector<std::vector<int>> readPassMatrix(std::string &path, std::string &classname);
	void updatePassMatrix(std::vector<std::vector<int>> &building_grid, glm::vec3 &position);
	void clearPassMatrix(std::vector<std::vector<int>> &building_grid, glm::vec3 &position);
	bool checkAvailability(std::vector<std::vector<int>> &building_grid, glm::vec3 &position);
	int getGridInfoFromPoint(float x, float y);
	static int JDIM = 1500;
	static int IDIM = 1000;
	static const int NDIR = 8;
	static int iDir[NDIR] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	static int jDir[NDIR] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	
	std::vector<glm::ivec2> pathFind(const Location &locStart, const Location &locFinish);

	//
	//	NODE CLASS --> source/pathfinding/node.cpp
	//

	class Node {
		// current position
		int rPos;
		int cPos;

		// total distance already travelled to reach the node
		int GValue;

		// FValue = GValue + remaining distance estimate
		int FValue;  // smaller FValue gets priority

	public:
		Node();
		Node(const Location &loc, int g, int f);

		Location getLocation() const { return Location(rPos, cPos); }
		int getGValue() const { return GValue; }
		int getFValue() const { return FValue; }
		void calculateFValue(const Location& locDest);
		void updateGValue(const int & i);

		// Estimation function for the remaining distance to the goal.
		const int & getHValue(const Location& locDest) const;

		// Determine FValue (in the priority queue)
		friend bool operator<(const Node & a, const Node & b) { return a.getFValue() > b.getFValue(); }
		~Node();
	};
}