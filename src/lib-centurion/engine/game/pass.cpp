#include "pass.h"

#include <map>

#include <grid.h>

#include <logger.h>

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

		vec2 GetGridCellPosition(glm::vec3 & objectPosition, int horizontal_size, int vertical_size)
		{
			int x = (int)objectPosition.x / GRID_CELL_SIZE - horizontal_size / 2;
			int y = (int)objectPosition.y / GRID_CELL_SIZE - vertical_size / 2;

			return vec2(x, y);
		}

		unsigned char getGridValueFromPoint(float x, float y) {
			int idx = (int)y / GRID_CELL_SIZE * GRID_SIZE_X + (int)x / GRID_CELL_SIZE;
			if (idx < 0 || idx >= GRID_ARRAY_SIZE) return 0;
			
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
		MapGrid()->update();
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

				if (pg->GetValueByCoordinates(ix, iy) == 1)
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
		MapGrid()->update();
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
					if (PASS_MATRIX[idx] == 1 && pg->GetValueByCoordinates(ix, iy) == 1)
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
		pg.Create(path, className);
		PASS_DATABASE[className] = pg;
	}

	PassGrid * GetPassGridPtr(const std::string className)
	{
		if (PASS_DATABASE.count(className) >= 1)
		{
			return &(PASS_DATABASE[className]);
		}
		return nullptr;
	}


	Pass::PassGrid::PassGrid(void) {}

	Pass::PassGrid::~PassGrid(void) {}

	unsigned int PassGrid::GetSizeX(void)
	{
		return (unsigned int)data[0].size();
	}

	unsigned int PassGrid::GetSizeY(void)
	{
		return (unsigned int)data.size();
	}

	unsigned char PassGrid::GetValueByCoordinates(const unsigned int X, const unsigned int Y)
	{
		return data[Y][X];
	}

	void PassGrid::Create(const std::string path, const std::string className)
	{
		ifstream fin(path);
		if (!fin.good()) 
		{
			stringstream ss;
			ss << "A problem occurred while reading " + className + " PASS file.";
			Logger::Warn(ss.str());
		}
		else 
		{
			string line, value;
			while (getline(fin, line)) 
			{
				if (line.length() > 0) 
				{
					vector<unsigned char> line_values;
					stringstream s(line);
					while (getline(s, value, ','))
					{
						unsigned char charValue = (unsigned char) stoi(value);
						line_values.push_back(charValue);
					}
					this->data.push_back(line_values);
				}
			}
		}
	}
};
