#pragma once

#include <global.hpp>
#include <objects.h>

#include "../game/game_object.h"

class Surface : public GObject
{
public:
	Surface();
	void createNoise();
	void updateGrid();
	void render(bool tracing);
	~Surface();
};

