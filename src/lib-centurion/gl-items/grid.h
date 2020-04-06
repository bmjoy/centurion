/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef GRID_H
#define GRID_H

#include "shader.h"
#include <game/game.h>

#ifndef CELLGRIDSIZE
#define CELLGRIDSIZE 20
#endif


#ifndef GRIDSIZE_X
#define GRIDSIZE_X MEDIUM_MAP_WIDTH / CELLGRIDSIZE
#endif 

#ifndef GRIDSIZE_Y
#define GRIDSIZE_Y MEDIUM_MAP_HEIGHT / CELLGRIDSIZE
#endif 

/// <summary>
/// This class is used to render the PASS grid.
/// </summary>
class Grid : public Shader
{
public:
	/// <summary>
	/// Constructor
	/// </summary>
	Grid();

	/// <summary>
	/// This function creates an empty grid texture based on the size of the pass grid;
	/// </summary>
	void create();
	
	/// <summary>
	/// This function resets the grid texture;
	/// </summary>
	void reset();

	/// <summary>
	/// This function updates the grid taking the data from astar::GridMatrix2D();
	/// </summary>
	void update();

	/// <summary>
	/// Render function;
	/// </summary>
	void render();


	void SetGridDataCell(const unsigned int idx, const unsigned int val);

	/// <summary>
	/// Destructor
	/// </summary>
	~Grid();
private:
	GLuint textureID;
	std::vector<unsigned char> gridData;
	
};

static Grid grid;

#endif