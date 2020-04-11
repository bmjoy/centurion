#include "pass.h"

#include <map>

#include <gl_grid.h>
#include <gl_items.h>

#include <logger.h>
#include <tinyxml2.h>
#include <encoding.hpp>

using namespace std;
using namespace glm;

namespace Pass
{
	// Private variables
	namespace
	{
		map<string, PassGrid> PASS_DATABASE = map<string, PassGrid>();

		unsigned char PASS_MATRIX[GRID_ARRAY_SIZE] = { 0 };
		unsigned char PASS_MATRIX_2D[GRID_ARRAY_SIZE] = { 0 };

		unsigned char PASSABLE = 0;
		unsigned char NOT_PASSABLE = 1;

		vec2 GetGridCellPosition(glm::vec3 & objectPosition, int horizontal_size, int vertical_size)
		{
			int x = (int)objectPosition.x / GRID_CELL_SIZE - horizontal_size / 2;
			int y = (int)objectPosition.y / GRID_CELL_SIZE - vertical_size / 2;

			return vec2(x, y);
		}

		unsigned char getGridValueFromPoint(float x, float y) 
		{
			int idx = (int)y / GRID_CELL_SIZE * GRID_SIZE_X + (int)x / GRID_CELL_SIZE;
			if (idx < 0 || idx >= GRID_ARRAY_SIZE) 
				return 0;
			return PASS_MATRIX[idx];
		}
	};

	unsigned int Pass::GetGridSizeX(void)
	{
		return GRID_SIZE_X;
	}

	unsigned int Pass::GetGridSizeY(void)
	{
		return GRID_SIZE_Y;
	}

	unsigned char * Pass::GetGridPtr(void)
	{
		return &PASS_MATRIX[0];
	}

	unsigned char GetGridValueByIndex(const unsigned int idx)
	{
		if (idx < 0 || idx >= GRID_ARRAY_SIZE) return 0;
		return PASS_MATRIX[idx];
	}

	unsigned char GetGrid2DValueByIndex(const unsigned int idx)
	{
		if (idx < 0 || idx >= GRID_ARRAY_SIZE) return 0;
		return PASS_MATRIX_2D[idx];
	}

	void Pass::SetGridCellValueByIndex(const unsigned int idx, const unsigned int val)
	{
		if (idx < 0 || idx >= GRID_ARRAY_SIZE) return;
		if (val != 0 && val != 1) return;

		PASS_MATRIX[idx] = val;
	}

	void Pass::ClearGrid(void)
	{
		for (int i = 0; i < GRID_ARRAY_SIZE; i++)
		{
			PASS_MATRIX[i] = 0;
		}
		GLItems::MapGrid()->update();
	}

	void Pass::UpdateObjectPass(PassGrid *pg, glm::vec3 & position, const int method)
	{
		int horizontal_size = pg->GetSizeX();
		int vertical_size = pg->GetSizeY();
		vec2 pos = GetGridCellPosition(position, horizontal_size, vertical_size);

		for (int iy = 0; iy < vertical_size; ++iy)
		{
			for (int ix = 0; ix < horizontal_size; ++ix)
			{
				int idx = (vertical_size - iy + (int)pos.y) * GRID_SIZE_X + ix + (int)pos.x;

				if (pg->GetValueByCoordinates(ix, iy) == NOT_PASSABLE)
				{
					if (idx >= 0 && idx < GRID_SIZE_X * GRID_SIZE_Y)
					{
						if (method == PASS_CLEAR)
						{
							PASS_MATRIX[idx] = 0;
						}
						else if (method == PASS_OVERLAP)
						{
							PASS_MATRIX[idx] = 1;
						}
					}
				}
			}
		}
		GLItems::MapGrid()->update();
	}

	bool CheckObjectPassAvailability(PassGrid *pg, glm::vec3 & position)
	{
		int horizontal_size = pg->GetSizeX();
		int vertical_size = pg->GetSizeY();
		vec2 pos = GetGridCellPosition(position, horizontal_size, vertical_size);

		bool bAvailable = true;

		for (int iy = 0; iy < vertical_size; iy++)
		{
			for (int ix = 0; ix < horizontal_size; ix++)
			{
				int idx = (vertical_size - iy + (int)pos.y) * GRID_SIZE_X + ix + (int)pos.x;
				if (idx >= 0 && idx < GRID_SIZE_X * GRID_SIZE_Y)
				{
					if (PASS_MATRIX[idx] == NOT_PASSABLE && pg->GetValueByCoordinates(ix, iy) == NOT_PASSABLE)
					{
						bAvailable = false;
						break;
					}
				}
				else
				{
					bAvailable = false;
					break;
				}
			}
		}
		return bAvailable;
	}

	void AddPassGrid(const std::string path, const std::string className)
	{
		PassGrid pg{};
		pg.ReadPass(path, className);
		PASS_DATABASE[className] = pg;
	}

	PassGrid * GetPassGridPtr(const std::string className)
	{
		if (PASS_DATABASE.count(className) >= 1)
		{
			return &(PASS_DATABASE[className]);
		}
		return new PassGrid();
	}


	Pass::PassGrid::PassGrid(void) {}

	Pass::PassGrid::~PassGrid(void) {}

	unsigned int PassGrid::GetSizeX(void)
	{
		return sizeX;
	}

	unsigned int PassGrid::GetSizeY(void)
	{
		return sizeY;
	}

	unsigned char PassGrid::GetValueByCoordinates(const unsigned int X, const unsigned int Y)
	{
		unsigned int idx = (sizeX * Y + X);

		if (gridData[idx] == (unsigned char)'1')
		{
			return 1;
		}
		else return 0;
	}

	void PassGrid::ReadPass(const std::string path, const std::string className)
	{
		sizeX = 0;
		sizeY = 0;
		try
		{
			tinyxml2::XMLDocument xmlFile(true, tinyxml2::COLLAPSE_WHITESPACE);
			xmlFile.LoadFile(path.c_str());

			xmlFile.FirstChildElement("pass")->QueryIntAttribute("x", &sizeX);
			xmlFile.FirstChildElement("pass")->QueryIntAttribute("y", &sizeY);
			string hexString = xmlFile.FirstChildElement("pass")->FirstChildElement("grid")->GetText();
			string binString = Encode::HexStrToBinStr(hexString);

			gridData = vector<unsigned char>(binString.begin(), binString.end());
		}
		catch (const std::exception&)
		{
			;
		}
	}

};
