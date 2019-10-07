#include "unit_functions.h"

#include "../pathfinding/a_star.h"
#include "../game/game_functions.h"

using namespace game;

void unit::updateZ(glm::vec3 &pos2d, glm::vec3 *pos3d) {
	float zNoise = mapgen::getNoiseEstimate(pos2d.x, pos2d.y);	
	(*pos3d).x = pos2d.x;
	(*pos3d).y = pos2d.y + zNoise;
}

void unit::updateFrame(float *creationTime, int *frame, int max_frames, int duration) {
	if (glfwGetTime() - *creationTime >= duration / 100.f) {
		(*frame)++;
		if (*frame == max_frames - 1) *frame = 0;
		(*creationTime) += duration / 100.f;
	}
}

bool unit::isInSelectionRect(std::array<float, 8> &coords){
	return(
		// are the 4 points in selection rectangle ?
		(coords[0] > SelRectCoords()->minX && coords[0] < SelRectCoords()->maxX &&
			coords[1] > SelRectCoords()->minY && coords[1] < SelRectCoords()->maxY) ||
		(coords[2] > SelRectCoords()->minX && coords[2] < SelRectCoords()->maxX &&
			coords[3] > SelRectCoords()->minY && coords[3] < SelRectCoords()->maxY) ||
		(coords[4] > SelRectCoords()->minX && coords[4] < SelRectCoords()->maxX &&
			coords[5] > SelRectCoords()->minY && coords[5] < SelRectCoords()->maxY) ||
		(coords[6] > SelRectCoords()->minX && coords[6] < SelRectCoords()->maxX &&
			coords[7] > SelRectCoords()->minY && coords[7] < SelRectCoords()->maxY) ||
		// does the sel. rectangle and hitbox rectangle intersect themselves?
		(coords[0] > SelRectCoords()->minX && coords[0] < SelRectCoords()->maxX &&
			coords[3] < SelRectCoords()->minY && coords[1] > SelRectCoords()->maxY) ||
		(coords[4] > SelRectCoords()->minX && coords[4] < SelRectCoords()->maxX &&
			coords[3] < SelRectCoords()->minY && coords[1] > SelRectCoords()->maxY) ||
		(coords[1] > SelRectCoords()->minY && coords[1] < SelRectCoords()->maxY &&
			coords[0] < SelRectCoords()->minX && coords[4] > SelRectCoords()->maxX) ||
		(coords[3] > SelRectCoords()->minY && coords[3] < SelRectCoords()->maxY &&
			coords[0] < SelRectCoords()->minX && coords[4] > SelRectCoords()->maxX)
		);
}

float unit::getAngle(std::vector<glm::ivec2> &path, int &n) {
	float angle = (float)atan2(path[n + 1].y - path[n].y, path[n + 1].x - path[n].x) * 180.0f / 3.14159265f;
	if (angle < 0) angle += 360.0f;
	return angle;
}

float unit::getDistance(std::vector<glm::ivec2> &path, int &n) {
	float deltaX = (float)path[n+1].x - (float)path[n].x;
	float deltaY = (float)path[n+1].y - (float)path[n].y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

float unit::getResDistance(std::vector<glm::ivec2> &path, int &n, glm::vec3 &pos2d) {
	float deltaX = (float)path[n+1].x - (int)pos2d.x / aStar::cellGridSize * aStar::cellGridSize;
	float deltaY = (float)path[n+1].y - (int)pos2d.y / aStar::cellGridSize * aStar::cellGridSize;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

std::vector<glm::ivec2> unit::getPath(glm::vec2 &start, glm::vec2 &end) {
	int jStart = (int)start.x / aStar::cellGridSize;
	int iStart = (int)start.y / aStar::cellGridSize;
	int jEnd = (int)end.x / aStar::cellGridSize;
	int iEnd = (int)end.y / aStar::cellGridSize;

	//fix pathfinding click to 1
	while (aStar::GridMatrix2D()[iEnd * aStar::gridWidth + jEnd] != 0) {
		iEnd--;
		jEnd--;
	}

	return aStar::pathFind(Location(iStart, jStart), Location(iEnd, jEnd));
}