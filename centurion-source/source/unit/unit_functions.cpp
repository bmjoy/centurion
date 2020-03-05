#include <unit>
#include <surface>
#include <game/strategy.h>
#include <game/game.h>
#include <pathfinding>


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