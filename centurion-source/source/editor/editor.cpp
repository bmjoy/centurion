#include <editor>
#include <surface>
#include <game>
#include <picking>
#include <interface>
#include <engine/camera.h>
#include <engine/mouse.h>
#include <engine/window.h>

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
		myWindow::BottomBarHeight = 0.f;
		myWindow::TopBarHeight = 30.f;
		game::setMinimapProjection();

		surface->reset();
		EDITOR_UI()->create();
		game::GAME()->selRectangle = gui::Rectangle();
		game::GAME()->selRectangle.create("border", 0, 0, 0, 0, "top-left", 0);
		Mouse::LeftHold = false;

		Camera::go_to_pos(1.f, 1.f);		

		editorIsCreated = true;
	}

	void Editor::run() {
		/* Keyboard control */
		handleKeyboardControls();
		if (!IsWindowOpened) { // TODO: merge all these in a function in Editor->Editor_functions.cpp
			Camera::keyboardControl();
		}

		/* If minimap is NOT active */
		if (!game::gameMinimapStatus) {
			if (!IsWindowOpened && Mouse::GetYPosition() < myWindow::Height - 30.f && !menuIsOpened) 
				Camera::mouseControl(game::cameraThreshold);
			view = Camera::calculateViewMatrix();
			proj = glb::cameraProjection;

			EDITOR_UI()->render(true);

			// apply game matrices
			obj::applyGameMatrices(&proj, &view);

			// picking
			if (!IsWindowOpened && !addingObject && !TerrainBrushIsActive) game::renderObjectsPicking();

			// rendering
			surface->render(false);
			game::renderObjects();
			if (!IsWindowOpened && !addingObject && !TerrainBrushIsActive) moveObjects();

			// apply menu matrices
			obj::applyMenuMatrices();

			EDITOR_UI()->render(false);
		}

		/* If minimap is active */
		else {
			view = mat4(1.0f);
			proj = glb::minimapProjection;

			// editor ui picking */
			EDITOR_UI()->render(true);

			if (game::MINIMAP()->getStatus()) game::MINIMAP()->render();
		
			if (!game::MINIMAP()->getStatus()) {
				obj::applyGameMatrices(&proj, &view);
				surface->render(false);
				game::renderObjects();
				game::MINIMAP()->create();
				obj::applyMenuMatrices();
			}
			
			EDITOR_UI()->render(false);

			if (leftClickID_UI == 0) game::goToPosition();
		}

		glb::cameraProjection = glm::ortho(0.0f, myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)game::mapWidth, (float)game::mapWidth);

		Mouse::RightClick = false;
		Mouse::LeftClick = false;
		Mouse::MiddleClick = false;
		KeyCode[GLFW_KEY_ESCAPE] = false;

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