#include <global.hpp>
#include <mapgen>
#include "../pathfinding/a_star.h"
#include <primitives>

/* UNIT FUNCTIONS */

namespace unit {
	void updateZ(glm::vec3 &pos2d, glm::vec3 *pos3d);
	void updateFrame(float *creationTime, int *frame, int max_frames, int duration);
	bool isInSelectionRect(std::array<float, 8> &coords);
	float getAngle(std::vector<glm::ivec2> &path, int &n);
	float getDistance(std::vector<glm::ivec2> &path, int &n);
	float getResDistance(std::vector<glm::ivec2> &path, int &n, glm::vec3 &pos2d);
	std::vector<glm::ivec2> getPath(glm::vec2 &start, glm::vec2 &end);
}