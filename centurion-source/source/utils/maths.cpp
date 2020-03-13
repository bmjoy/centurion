#include <maths.h>

using namespace std;
using namespace glm;

namespace math {

	float euclidean_distance(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	float ellipse_distance(float x, float y, float cx, float cy, float r) {
		return ((x - cx)*(x - cx) / (r*r) + 1.5f * (y - cy) * (y - cy) / (r * r));
	}
	
	array<float, 8> get_rectangle_coords(float x0, float y0, float w, float h) {
		return { x0, y0, x0, y0 - h, x0 + w, y0 - h, x0 + w, y0 };
	}
	
	
};