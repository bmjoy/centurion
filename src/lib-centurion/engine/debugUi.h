/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#pragma once

#include <ui.h>

class DebugUI {
public:
	DebugUI();
	void setStatus(bool b) { debuguiIsActive = b; }
	bool getStatus() { return debuguiIsActive; }
	void Create();
	void Render();
	~DebugUI();
private:
	// text
	std::array<std::string, 10> staticTextList;
	std::array<std::string, 10> dynamicTextList;
	std::vector<gui::SimpleText> staticText;
	gui::SimpleText dynamicText;
	gui::SimpleText debugText[3];
	float startX, startY, deltaY;
	int n;
	bool debuguiIsActive;
	bool isCreated;
	// background
	gui::Rectangle back;
	std::string currentLan;
};