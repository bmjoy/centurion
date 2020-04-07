#pragma once

#include <header.h>

namespace astar {

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

	std::vector<std::vector<unsigned int>> readPassMatrix(std::string &path, std::string &classname);

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