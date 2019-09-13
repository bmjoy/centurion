#pragma once

#include <ctime>
#include <string>
#include <queue>

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

