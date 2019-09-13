#pragma once

#include <global.hpp>

#include "node.h"

class aStar {
public:
	aStar();
	std::vector<glm::ivec2>  pathFind(const Location &locStart, const Location &locFinish);
	~aStar();
private:
	int JDIM;
	int IDIM;
	int NDIR;
	int iDir[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	int jDir[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	std::vector<glm::ivec2> finalPath;
};