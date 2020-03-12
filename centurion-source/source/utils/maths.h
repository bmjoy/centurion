#pragma once

#include <headers>

// cpp file --> source/math.cpp

using namespace std;
using namespace glm;

namespace math {
	float euclidean_distance(float x1, float y1, float x2, float y2);
	float ellipse_distance(float x, float y, float cx, float cy, float r);
	array<float, 8> get_rectangle_coords(float x0, float y0, float w, float h); // empty rectangle creation
};