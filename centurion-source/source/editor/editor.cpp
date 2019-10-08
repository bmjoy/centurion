#include <editor>
#include <surface>
#include <engine>
#include <picking>
#include "../interface/editor_menu.h"

using namespace glb;

namespace editor {
	Editor::Editor(){
		editorIsCreated = false;
	}
	void Editor::create() {
		resetPicking();
		surface = new Surface();
		camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
		menu = new editor::EditorMenu();
		circle = gui::Circle();

		surface->reset();
		menu->create();
		
		circle.create("border", 0.f, 0.f, 300.f, 200.f, 8.f, "center");
		editorIsCreated = true;
	}
	void Editor::run() {
		camera->keyboardControl();
		camera->mouseControl(2.0f);

		proj = GLB::CAMERA_PROJECTION;
		view = camera->calculateViewMatrix();

		obj::applyGameMatrices(&proj, &view);
		surface->render(false);

		//-----------------------------

		obj::applyMenuMatrices();

		/* temporary : it will be in editor UI */
		circle.render(glm::vec4(255.f), getParam("mouse-x-position"), getParam("mouse-y-position"));
		menu->render(true);
		menu->render(false);

		GLB::CAMERA_PROJECTION = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)GAME::MAP_WIDTH, (float)GAME::MAP_WIDTH);
	}

	Editor::~Editor(){
		delete surface;
		delete menu;
		delete camera;
	}
}