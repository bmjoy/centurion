#pragma once

#include <global.hpp>

#include "node.h"

namespace aStar {

	static int cellGridSize = 20;
	static const int gridWidth = 1500;
	static const int gridHeight = 1000;
	static int gridMatrix[gridWidth * gridHeight] = { 0 };
	static int gridMatrix2D[gridWidth * gridHeight] = { 0 };
	static int closedNodes[gridWidth * gridHeight] = { 0 };
	static int openNodes[gridWidth * gridHeight] = { 0 };
	static int dirMap[gridWidth * gridHeight] = { 0 };
	int *GridMatrix();
	int *GridMatrix2D();
	int *ClosedNodes();
	int *OpenNodes();
	int *DirMap();
	void updatePassMatrix(std::vector<std::vector<int>> mat, glm::vec2 position);
	int getGridInfoFromPoint(float x, float y);

	static int JDIM = 1500;
	static int IDIM = 1000;
	static const int NDIR = 8;
	static int iDir[NDIR] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	static int jDir[NDIR] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	std::vector<glm::ivec2> pathFind(const Location &locStart, const Location &locFinish);
}