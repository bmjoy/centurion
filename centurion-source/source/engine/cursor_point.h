#pragma once

#include "../primitives/image.h"

class CPoint
{
public:
	CPoint();
	void create();
	void render();
	~CPoint();
private:
	Image img;
	float xPosGrid, yPosGrid;
};

