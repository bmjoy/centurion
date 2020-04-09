/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <array>

/// <summary>
/// This namespace contains some mathematical functions.
/// </summary>
namespace Math {
	
	/// <summary>
	/// This function calculates the euclidean distance between two points
	/// </summary>
	/// <param name="x1">X coordinate of the first point</param>
	/// <param name="y1">Y coordinate of the first point</param>
	/// <param name="x2">X coordinate of the second point</param>
	/// <param name="y2">Y coordinate of the second point</param>
	static float euclidean_distance(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}
	
	/// <summary>
	/// This function calculates the distance between two points within an ellipse
	/// </summary>
	/// <param name="x">X coordinate of the first point</param>
	/// <param name="y">Y coordinate of the first point</param>
	/// <param name="cx">X coordinate of the second point</param>
	/// <param name="cy">Y coordinate of the second point</param>
	/// <param name="r">Ellipse radius</param>
	static float ellipse_distance(float x, float y, float cx, float cy, float r) {
		return ((x - cx)*(x - cx) / (r*r) + 1.5f * (y - cy) * (y - cy) / (r * r));
	}
	
	/// <summary>
	/// This function creates a rectangle into a specified point
	/// </summary>
	/// <param name="x0">X coordinate of the rectangle</param>
	/// <param name="y0">Y coordinate of the rectangle</param>
	/// <param name="w">Rectangle width</param>
	/// <param name="h">Rectangle height</param>
	static std::array<float, 8> get_rectangle_coords(float x0, float y0, float w, float h) {
		return { x0, y0, x0, y0 - h, x0 + w, y0 - h, x0 + w, y0 };
	}

	// Map Noise Generation Functions

	struct Triangle {
		glm::vec3 a, b, c;
	};

	static float random2D(const glm::vec2 st) {
		const float value = glm::fract(sin(glm::dot(st, glm::vec2(12.9898f, 78.233f))) * 43758.5453123f);
		return value;
	}

	static float interpolate(const float a, const float b, const float x) {  // cosine interpolation
		float ft = x * 3.1415927f;
		float f = (1.0f - cos(ft)) * 0.5f;
		return  a * (1 - f) + b * f;
	}

	static float interpolated_noise(const int ind, const float x, const float y) {
		int integer_X = int(floor(x));
		float fractional_X = glm::fract(x);
		int integer_Y = int(floor(y));
		float fractional_Y = glm::fract(y);
		glm::vec2 randomInput = glm::vec2(integer_X, integer_Y);
		float v1 = random2D(randomInput);
		float v2 = random2D(randomInput + glm::vec2(1.0, 0.0));
		float v3 = random2D(randomInput + glm::vec2(0.0, 1.0));
		float v4 = random2D(randomInput + glm::vec2(1.0, 1.0));
		float i1 = interpolate(v1, v2, fractional_X);
		float i2 = interpolate(v3, v4, fractional_X);
		return interpolate(i1, i2, fractional_Y);
	}

	static float perlin_noise(float x, float y, float xy_scale, float z_scale, float x_seed, float y_seed, const unsigned int map_width, const unsigned int map_height) {
		z_scale *= map_width;
		xy_scale /= map_height;

		x += x_seed;
		y += y_seed;
		x *= xy_scale;
		y *= xy_scale;
		int numOctaves = 1;
		float persistence = 0.5;
		float total = 0,
			frequency = pow(2.0f, numOctaves),
			amplitude = 1;
		for (int i = 0; i < numOctaves; ++i) {
			frequency /= 2;
			amplitude *= persistence;
			total += interpolated_noise(int(i % 10), x / frequency, y / frequency) * amplitude;
		}
		float z = total / frequency * z_scale;

		return z;
	}

	static float smooth_noise(const float y, const float z, const float map_height, const float alpha = 0.005f, const float beta = 1.0f) {
		float zSmoothed;
		zSmoothed = z * beta * tanh(y * alpha) * tanh((map_height - y) * alpha);
		return zSmoothed;
	}

	static std::vector<glm::ivec2> get_adjacent_vertices(glm::ivec2 pos, int triangle_width) {
		int gap = triangle_width;
		std::vector<glm::ivec2> output;
		output = {
			glm::ivec2(pos.x - gap, pos.y), // left
			glm::ivec2(pos.x - gap, pos.y + gap), // up-left
			glm::ivec2(pos.x, pos.y + gap), // up
			glm::ivec2(pos.x + gap, pos.y), // right 
			glm::ivec2(pos.x + gap, pos.y - gap), // down-right
			glm::ivec2(pos.x, pos.y - gap), // down
		};
		return output;
	}

	static glm::vec3 calculate_triangle_normal(Triangle T) {
		glm::vec3 N = glm::cross(T.c - T.a, T.b - T.a);
		return N;
	}
};