/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

/// <summary>
/// 
/// This namespace contains all methods about the file PASS of each object
/// and the PASS GRID MATRIX, a huge matrix of 0/1 that indicates if that point
/// is passable or not. The idea is to store every file PASS in a std::map 
/// where the key is the class name. In each object there will be a pointer 
/// to the corresponding file PASS.
/// 
/// The file PASS contains hexadecimal (HEX) values. They are converted to 
/// binary (e.g. A = 1010) in order to obtain four matrix cells.
/// 
/// The map PASS grids (or matrices) are (1D) arrays of unsigned char values.
/// To obtain a value using a couple of coordinates (x,y) we perform an
/// arithmetic operation (every 2D matrix can be converted to 1D array).
/// 
/// It's very important to set correctly the const size values.
/// 
/// The arrays, that are quite onerous and costly in terms of memory, are 
/// stored in the .cpp file, in the anonymous namespace. We get access 
/// to those arrays using pointer methods.
/// 
/// </summary>

#include "map.h"
#include <header.h>

#ifndef PASS_CLEAR
#define PASS_CLEAR 0
#endif

#ifndef PASS_OVERLAP
#define PASS_OVERLAP 1
#endif 

#ifndef GRID_CELL_SIZE
#define GRID_CELL_SIZE 20
#endif

#ifndef GRID_SIZE_X
#define GRID_SIZE_X MEDIUM_MAP_WIDTH / GRID_CELL_SIZE
#endif 

#ifndef GRID_SIZE_Y
#define GRID_SIZE_Y MEDIUM_MAP_HEIGHT / GRID_CELL_SIZE
#endif 

#ifndef GRID_ARRAY_SIZE
#define GRID_ARRAY_SIZE GRID_SIZE_X * GRID_SIZE_Y
#endif

namespace Pass
{
	class PassGrid
	{
	public:
		PassGrid(void);
		~PassGrid(void);

		unsigned int GetSizeX(void);

		unsigned int GetSizeY(void);

		unsigned char GetValueByCoordinates(const unsigned int X, const unsigned int Y);

		void Create(const std::string path, const std::string className);
	private:
		std::vector<std::vector<unsigned char>> data;
	};

	unsigned int GetGridSizeX(void);

	unsigned int GetGridSizeY(void);

	unsigned char * GetGridPtr(void);

	unsigned char GetGridValueByIndex(const unsigned int idx);

	unsigned char GetGrid2DValueByIndex(const unsigned int idx);


	//void SetGridCellValueByCoordinates(const unsigned int x, const unsigned int y, const unsigned int val);

	void SetGridCellValueByIndex(const unsigned int idx, const unsigned int val);

	void ClearGrid(void);

	void UpdateObjectPass(PassGrid *pg, glm::vec3 &position, const int method);

	bool CheckObjectPassAvailability(PassGrid *pg, glm::vec3 &position);

	void AddPassGrid(const std::string path, const std::string className);

	PassGrid *GetPassGridPtr(const std::string className);
};

