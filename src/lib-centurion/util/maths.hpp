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

};