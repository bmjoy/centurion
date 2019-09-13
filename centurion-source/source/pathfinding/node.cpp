#include "node.h"

Node::Node() {
}

Node::Node(const Location &loc, int g, int f) {
	rPos = loc.row;
	cPos = loc.col;
	GValue = g;
	FValue = f;
}

void Node::calculateFValue(const Location &locDest) {
	FValue = GValue + getHValue(locDest);
}

void Node::updateGValue(const int &i) { // i: direction
	GValue += ((i % 2 == 0 ? 10 : 14));
}

const int &Node::getHValue(const Location& locDest) const {
	static int rd, cd, d;
	rd = locDest.row - rPos;
	cd = locDest.col - cPos;

	// Euclidian Distance
	//d = static_cast<int>(sqrt((double)(rd*rd+cd*cd)));
	d = 14 * (int)sqrt(rd*rd+cd*cd);

	// Manhattan distance
	//d = 10 * (abs(rd) + abs(cd));

	// Chebyshev distance
	//d = 10 * (std::max(abs(rd), abs(cd)));

	return(d);
}

Node::~Node() {
}
