#pragma once

#include <global.hpp>
#include "../gui/simple_text.h"
#include "../gui/rectangle.h"

class DebugUI
{
public:
	DebugUI();
	void create();
	void render(int fps, int mpfs, int selUnits);
	~DebugUI();
private:
	// text
	std::array<std::string, 10> staticTextList;
	std::array<std::string, 10> dynamicTextList;
	std::vector<gui::SimpleText> staticText;
	gui::SimpleText dynamicText;
	gui::SimpleText debugText;
	float startX, startY, deltaY;
	int n;

	// background
	gui::Rectangle back;

};

