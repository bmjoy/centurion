#include "editor.h"



Editor::Editor(){
	editorIsCreated = false;
	pickingID = 1;
}

void Editor::create() {

	/*
		instructions
	*/

	surface = new Surface();
	//

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	menu = editor::Menu();
	menu.create(&pickingID);

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

	menu.render(true);
	menu.render(false);

	obj::applyMenuMatrices();
	GLB::CAMERA_PROJECTION = glm::ortho(0.0f, (float)GLB::WINDOW_WIDTH_ZOOMED, 0.0f, (float)GLB::WINDOW_HEIGHT_ZOOMED, -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
}

Editor::~Editor(){
	delete surface;
}
