#pragma once

#include <global.hpp>
#include <objects.h>

#include "../game/game_object.h"
#include "../engine/window.h"
#include "grid.h"

class Surface : public GObject
{
public:
	Surface();
	void create();
	void set_window(myWindow *wnd) { window = wnd; }
	void render(glm::mat4 projMat, glm::mat4 viewMat, bool tracing);
	~Surface();

private:
	myWindow *window;
	Grid grid;
};

