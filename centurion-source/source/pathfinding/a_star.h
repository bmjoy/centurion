#pragma once

#include <global.hpp>

#include "node.h"

namespace aStar {

	static int JDIM = 1500;
	static int IDIM = 1000;
	static const int NDIR = 8;
	static int iDir[NDIR] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	static int jDir[NDIR] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	std::vector<glm::ivec2> pathFind(const Location &locStart, const Location &locFinish);
}