#include <object>
#include <surface>
#include <game>
#include <pathfinding>

using namespace game;
using namespace std;
using namespace glm;

namespace unit {
	void updateZ(vec3 &pos2d, vec3 *pos3d) {
		float zNoise = mapgen::getNoiseEstimate(pos2d.x, pos2d.y);	
		(*pos3d).x = pos2d.x;
		(*pos3d).y = pos2d.y + zNoise;
	}

	void updateFrame(float *creationTime, int *frame, int max_frames, int duration) {
		if (glfwGetTime() - *creationTime >= duration / 100.f) {
			(*frame)++;
			if (*frame == max_frames - 1) *frame = 0;
			(*creationTime) += duration / 100.f;
		}
	}

	bool isInSelectionRect(array<float, 8> &coords){
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

	float getAngle(vector<ivec2> &path, int &n) {
		float angle = (float)atan2(path[n + 1].y - path[n].y, path[n + 1].x - path[n].x) * 180.0f / 3.14159265f;
		if (angle < 0) angle += 360.0f;
		return angle;
	}

	float getDistance(vector<ivec2> &path, int &n) {
		float deltaX = (float)path[n+1].x - (float)path[n].x;
		float deltaY = (float)path[n+1].y - (float)path[n].y;
		return sqrt(deltaX * deltaX + deltaY * deltaY);
	}

	float getResDistance(vector<ivec2> &path, int &n, vec3 &pos2d) {
		float deltaX = (float)path[n+1].x - (int)pos2d.x / astar::cellGridSize * astar::cellGridSize;
		float deltaY = (float)path[n+1].y - (int)pos2d.y / astar::cellGridSize * astar::cellGridSize;
		return sqrt(deltaX * deltaX + deltaY * deltaY);
	}

	vector<ivec2> getPath(vec2 &start, vec2 &end) {
		int jStart = (int)start.x / astar::cellGridSize;
		int iStart = (int)start.y / astar::cellGridSize;
		int jEnd = (int)end.x / astar::cellGridSize;
		int iEnd = (int)end.y / astar::cellGridSize;

		//fix pathfinding click to 1
		while (astar::GridMatrix2D()[iEnd * astar::gridWidth + jEnd] != 0) {
			iEnd--;
			jEnd--;
		}

		return astar::pathFind(astar::Location(iStart, jStart), astar::Location(iEnd, jEnd));
	}
}