#pragma once

#include <gui>
#include <mapgen>

class CPoint
{
public:
	CPoint();
	void create();
	void render();
	~CPoint();
private:
	gui::Image img;
	float xPosGrid, yPosGrid;
};

