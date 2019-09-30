#pragma once

#include <global.hpp>
#include "../engine/camera.h"
#include "../terrain/surface.h"

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

	bool editorIsCreated;
};

