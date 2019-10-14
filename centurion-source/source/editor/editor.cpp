#include <editor>
#include <surface>
#include <engine>
#include <game>
#include <picking>
#include <interface>

using namespace glb;
using namespace engine;
using namespace game;

namespace editor {
	Editor::Editor(){
		editorIsCreated = false;
	}
	void Editor::create() {
		resetPicking();
		surface = new Surface();
		menu = new editor::EditorMenu();
		circle = gui::Circle();

		surface->reset();
		menu->create();
		
		circle.create("border", 0.f, 0.f, 300.f, 200.f, 8.f, "center");
		editorIsCreated = true;
	}
	void Editor::run() {
		CAMERA()->keyboardControl();
		CAMERA()->mouseControl(2.0f);

		// editor ui picking */
		obj::applyMenuMatrices();
		menu->render(true);

		//-----------------------------
		/* normal rendering */
		proj = glb::cameraProjection;
		view = CAMERA()->calculateViewMatrix();
		obj::applyGameMatrices(&proj, &view);
		surface->render(false);

		// editor UI normal rendering
		obj::applyMenuMatrices();
		circle.render(glm::vec4(255.f), getParam("mouse-x-position"), getParam("mouse-y-position")); /* temporary : it will be in editor UI */
		menu->render(false);
		
		glb::cameraProjection = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)mapWidth, (float)mapWidth);

		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false); // temporary
	}

	Editor::~Editor(){
		delete surface;
		delete menu;
	}
}