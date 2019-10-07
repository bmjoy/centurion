#include "editor.h"

using namespace glb;

Editor::Editor(){
	editorIsCreated = false;
	pickingID = 1;
}

void Editor::create() {

	/*
		instructions
	*/

	surface = new Surface();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	menu = editor::Menu();
	menu.create(&pickingID);

	circle = gui::Circle();
	circle.create("border", 0.f, 0.f, 300.f, 200.f, 8.f, "center");

	editorIsCreated = true;
}

void Editor::run() {
	camera.keyboardControl();
	camera.mouseControl(2.0f);

	proj = GLB::CAMERA_PROJECTION;
	view = camera.calculateViewMatrix();

	obj::applyGameMatrices(&proj, &view);
	surface->render(false);

	//-----------------------------

	obj::applyMenuMatrices();

	/* temporary : it will be in editor UI */
	circle.render(glm::vec4(255.f), getParam("mouse-x-position"), getParam("mouse-y-position"));
	menu.render(true);
	menu.render(false);

	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
}

Editor::~Editor(){
	delete surface;
}
