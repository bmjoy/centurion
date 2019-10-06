#pragma once

#include <global.hpp>
#include "../engine/camera.h"
#include "../surface/surface.h"
#include "../interface/editor_menu.h"

class Editor
{
public:
	Editor();
	bool editor_is_created() { return editorIsCreated; }
	void create();
	void run();
	~Editor();
private:
	
	Surface *surface;
	Camera camera;
	glm::mat4 proj, view;
	editor::Menu menu;

	gui::Circle circle;

	int pickingID;

	bool editorIsCreated;
};

