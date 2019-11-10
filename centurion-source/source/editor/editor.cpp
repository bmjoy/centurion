#include <editor>
#include <surface>
#include <engine>
#include <game>
#include <picking>
#include <interface>

using namespace glb;
using namespace engine;

namespace editor {

	Editor::Editor(){
		editorIsCreated = false;	
	}

	void Editor::create() {
		resetPicking();
		surface = new Surface();

		game::GAME()->reset();
		setParam("ui-bottom-height", 0.f);
		setParam("ui-top-height", 30.f);
		game::setMinimapProjection();

		surface->reset();
		EDITOR_UI()->create();
		game::GAME()->selRectangle = gui::Rectangle();
		game::GAME()->selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
		setBoolean("mouse-left-pressed", false);

		CAMERA()->go_to_pos(1.f, 1.f);

		editorIsCreated = true;
	}

	void Editor::run() {
		/* Keyboard control */
		if (!IsWindowOpened) { // TODO: merge all these in a function in Editor->Editor_functions.cpp
			CAMERA()->keyboardControl();
			handleKeyboardControls();
		}

		/* If minimap is NOT active */
		if (!game::gameMinimapStatus) {
			if (!IsWindowOpened && getParam("mouse-y-position") < getParam("window-height") - 30.f && !menuIsOpened) 
				CAMERA()->mouseControl(game::cameraThreshold);
			view = CAMERA()->calculateViewMatrix();
			proj = glb::cameraProjection;
		}

		/* If minimap is active */
		else {
			view = mat4(1.0f);
			proj = glb::minimapProjection;
		}

		// editor ui picking */
		EDITOR_UI()->render(true);

		//-----------------------------
		/* normal rendering */
		obj::applyGameMatrices(&proj, &view);

		if (!IsWindowOpened && !addingObject && !TerrainBrushIsActive) game::renderObjectsPicking();

		surface->render(false);

		if (!IsWindowOpened && !addingObject && !TerrainBrushIsActive) moveObjects();

		game::renderObjects();

		// editor UI normal rendering
		obj::applyMenuMatrices();
		EDITOR_UI()->render(false);

		if(game::gameMinimapStatus) game::goToPosition();
		glb::cameraProjection = glm::ortho(0.0f, getParam("window-width-zoomed"), 0.0f, getParam("window-height-zoomed"), -(float)game::mapWidth, (float)game::mapWidth);

		setBoolean("mouse-right", false);
		setBoolean("mouse-left", false); 
		setBoolean("mouse-middle", false);

		if (IsWindowOpened) {
			KeyCode[GLFW_KEY_BACKSPACE] = false;
			KeyCode[GLFW_KEY_DELETE] = false;
			KeyCode[GLFW_KEY_UP] = false;
			KeyCode[GLFW_KEY_DOWN] = false;
			KeyCode[GLFW_KEY_LEFT] = false;
			KeyCode[GLFW_KEY_RIGHT] = false;
		}
	}

	Editor::~Editor(){
		delete surface;
	}
}