#pragma once

#include <global.hpp>
#include <objects>

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

