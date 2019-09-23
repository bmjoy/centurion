#include "unit_functions.h"

void unit::updateZ(glm::vec3 &pos2d, glm::vec3 *pos3d) {
	float zNoise = mapgen::generateNoise(pos2d).zNoise;	
	(*pos3d).x = pos2d.x;
	(*pos3d).y = pos2d.y + zNoise - PATH::CELL_GRID_SIZE / 2;
}

void unit::updateFrame(USprite *sprite, float *creationTime, int *frame, int max_frames, int duration) {
	if (glfwGetTime() - *creationTime >= duration/100.f) {
		(*frame)++;
		if (*frame == max_frames - 1) *frame = 0;
		(*creationTime) += duration / 100.f;
	}
	(*sprite).set_frame(*frame, max_frames);
}

void unit::updateDirection(USprite *sprite, int &direction) {
	(*sprite).set_direction(direction);
}

bool unit::isInSelectionRect(std::array<float, 8> &coords){
	return(
		// are the 4 points in selection rectangle ?
		(coords[0] > GLB::SEL_RECT_COORDS.minX && coords[0] < GLB::SEL_RECT_COORDS.maxX &&
			coords[1] > GLB::SEL_RECT_COORDS.minY && coords[1] < GLB::SEL_RECT_COORDS.maxY) ||
		(coords[2] > GLB::SEL_RECT_COORDS.minX && coords[2] < GLB::SEL_RECT_COORDS.maxX &&
			coords[3] > GLB::SEL_RECT_COORDS.minY && coords[3] < GLB::SEL_RECT_COORDS.maxY) ||
		(coords[4] > GLB::SEL_RECT_COORDS.minX && coords[4] < GLB::SEL_RECT_COORDS.maxX &&
			coords[5] > GLB::SEL_RECT_COORDS.minY && coords[5] < GLB::SEL_RECT_COORDS.maxY) ||
		(coords[6] > GLB::SEL_RECT_COORDS.minX && coords[6] < GLB::SEL_RECT_COORDS.maxX &&
			coords[7] > GLB::SEL_RECT_COORDS.minY && coords[7] < GLB::SEL_RECT_COORDS.maxY) ||
		// does the sel. rectangle and hitbox rectangle intersect themselves?
		(coords[0] > GLB::SEL_RECT_COORDS.minX && coords[0] < GLB::SEL_RECT_COORDS.maxX &&
			coords[3] < GLB::SEL_RECT_COORDS.minY && coords[1] > GLB::SEL_RECT_COORDS.maxY) ||
		(coords[4] > GLB::SEL_RECT_COORDS.minX && coords[4] < GLB::SEL_RECT_COORDS.maxX &&
			coords[3] < GLB::SEL_RECT_COORDS.minY && coords[1] > GLB::SEL_RECT_COORDS.maxY) ||
		(coords[1] > GLB::SEL_RECT_COORDS.minY && coords[1] < GLB::SEL_RECT_COORDS.maxY &&
			coords[0] < GLB::SEL_RECT_COORDS.minX && coords[4] > GLB::SEL_RECT_COORDS.maxX) ||
		(coords[3] > GLB::SEL_RECT_COORDS.minY && coords[3] < GLB::SEL_RECT_COORDS.maxY &&
			coords[0] < GLB::SEL_RECT_COORDS.minX && coords[4] > GLB::SEL_RECT_COORDS.maxX)
		);
}

float unit::getAngle(std::vector<glm::ivec2> &path, int &n) {
	float angle = atan2(path[n + 1].y - path[n].y, path[n + 1].x - path[n].x) * 180 / 3.14159265;
	if (angle < 0) angle += 360.0f;
	return angle;
}

float unit::getDistance(std::vector<glm::ivec2> &path, int &n) {
	float deltaX = path[n+1].x - path[n].x;
	float deltaY = path[n+1].y - path[n].y;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

float unit::getResDistance(std::vector<glm::ivec2> &path, int &n, glm::vec3 &pos2d) {
	float deltaX = path[n+1].x - (int)pos2d.x / PATH::CELL_GRID_SIZE * PATH::CELL_GRID_SIZE;
	float deltaY = path[n+1].y - (int)pos2d.y / PATH::CELL_GRID_SIZE * PATH::CELL_GRID_SIZE;
	return sqrt(deltaX * deltaX + deltaY * deltaY);
}

std::vector<glm::ivec2> unit::getPath(glm::vec2 &start, glm::vec2 &end) {
	int jStart = (int)start.x / PATH::CELL_GRID_SIZE;
	int iStart = (int)start.y / PATH::CELL_GRID_SIZE;
	int jEnd = (int)end.x / PATH::CELL_GRID_SIZE;
	int iEnd = (int)end.y / PATH::CELL_GRID_SIZE;

	//fix pathfinding click to 1
	while (PATH::GRID_MATRIX_2D[iEnd][jEnd] != 0) {
		iEnd--;
		jEnd--;
	}

	return aStar::pathFind(Location(iStart, jStart), Location(iEnd, jEnd));
}