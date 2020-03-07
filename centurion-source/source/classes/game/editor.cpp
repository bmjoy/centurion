#include <game/editor.h>
#include <surface>
#include <game/strategy.h>
#include <picking>
#include <interface>
#include <engine/camera.h>
#include <engine/mouse.h>
#include <engine/window.h>
#include <engine/engine.h>

using namespace glb;
using namespace engine;

#pragma region Static variables

#pragma endregion


Editor::Editor() {}

void Editor::Create() {
	resetPicking();
	resetPicking_UI();

	Strategy::reset();
	myWindow::BottomBarHeight = 0.f;
	myWindow::TopBarHeight = 30.f;
	setMinimapProjection();

	Surface::Reset();
	editor::EDITOR_UI()->create();

	SelectionRectangle::Create();

	Mouse::LeftHold = false;

	Camera::GoToPoint(1.f, 1.f);

	isCreated = true;
	Minimap::Update();
}

void Editor::Run() {
	/* Keyboard control */
	handleKeyboardControls();
	if (!editor::IsWindowOpened) { // TODO: merge all these in a function in Editor->Editor_functions.cpp
		Camera::keyboardControl();
	}

	/* If minimap is NOT active */
	if (Minimap::IsActive() == false) {
		if (!editor::IsWindowOpened && Mouse::GetYPosition() < myWindow::Height - 30.f && !editor::menuIsOpened)
			Camera::mouseControl();
		viewMatrix = Camera::calculateViewMatrix();
		projectionMatrix = glb::cameraProjection;

		editor::EDITOR_UI()->render(true);

		// apply game matrices
		obj::applyGameMatrices(&projectionMatrix, &viewMatrix);

		// picking
		if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) RenderObjectsPicking();

		// rendering
		Surface::Render(false);
		RenderObjects();
		if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) editor::moveObjects();

		// apply menu matrices
		obj::applyMenuMatrices();

		editor::EDITOR_UI()->render(false);
	}

	/* If minimap is active */
	else {
		viewMatrix = mat4(1.0f);
		projectionMatrix = glb::minimapProjection;

		// editor ui picking */
		editor::EDITOR_UI()->render(true);

		if (Minimap::IsCreated()) Minimap::Render();

		if (!Minimap::IsCreated()) {
			obj::applyGameMatrices(&projectionMatrix, &viewMatrix);
			Surface::Render(false);
			RenderObjects();
			Minimap::Create();
			obj::applyMenuMatrices();
		}

		editor::EDITOR_UI()->render(false);

		if (leftClickID_UI == 0) GoToPointFromMinimap();
	}

	glb::cameraProjection = glm::ortho(0.0f, myWindow::WidthZoomed, 0.0f, myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH);

	Mouse::RightClick = false;
	Mouse::LeftClick = false;
	Mouse::MiddleClick = false;
	KeyCode[GLFW_KEY_ESCAPE] = false;

	if (editor::IsWindowOpened) {
		KeyCode[GLFW_KEY_BACKSPACE] = false;
		KeyCode[GLFW_KEY_DELETE] = false;
		KeyCode[GLFW_KEY_UP] = false;
		KeyCode[GLFW_KEY_DOWN] = false;
		KeyCode[GLFW_KEY_LEFT] = false;
		KeyCode[GLFW_KEY_RIGHT] = false;
	}
}



void Editor::handleKeyboardControls() {

	using namespace editor;


	//CTRL Hotkeys
	if (!IsWindowOpened) {
		if (KeyCode[GLFW_KEY_LEFT_CONTROL] || KeyCode[GLFW_KEY_RIGHT_CONTROL]) {
			if (KeyCode[GLFW_KEY_N]) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
			if (KeyCode[GLFW_KEY_O]) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
			if (KeyCode[GLFW_KEY_S]) { saveCurrentScenario(currentMapName); }
			if (KeyCode[GLFW_KEY_A]) { TerrainBrushIsActive = false; TerrainBrushWindowIsOpen = false; AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
			if (KeyCode[GLFW_KEY_T]) { AddObjectWindowIsOpen = false; TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen; }
		}
		if (KeyCode[GLFW_KEY_DELETE]) {
			/*if (buildings.count(leftClickID) > 0) {
				if (buildings[leftClickID].isSelected()) {
					if (buildings[leftClickID].is_independent()) {
						if (buildings[leftClickID].buildingsInSettlementCount() > 0) {
							buildings[leftClickID].setWaitingToBeErased(true);
							Q_WINDOW()->setQuestion("QUESTION_deleteAll");
						}
						else {
							cout << "[DEBUG] Settlement " << buildings[leftClickID].get_name() << " deleted!\n";
							buildings[leftClickID].clear_pass();
							buildings.erase(leftClickID);
						}
					}
					else {
						cout << "[DEBUG] Building " << buildings[leftClickID].get_name() << " deleted!\n";
						buildings[leftClickID].clear_pass();
						buildings.erase(leftClickID);
					}
				}
			}*/
		}
		if (KeyCode[GLFW_KEY_SPACE] || Mouse::MiddleClick) {
			if (Minimap::IsActive()) Minimap::Disable();
			else Minimap::Enable();
			Minimap::IsActive() ? std::cout << "[DEBUG] Minimap ON!\n" : std::cout << "[DEBUG] Minimap OFF!\n";
		}
		if (KeyCode[GLFW_KEY_Z]) {
			Surface::Wireframe = !Surface::Wireframe;
			Surface::Wireframe ? std::cout << "[DEBUG] Wireframe ON!\n" : std::cout << "[DEBUG] Wireframe OFF! \n";
		}
		// Grid
		if (KeyCode[GLFW_KEY_G]) {
			if (Surface::IsGridEnabled()) Surface::DisableGrid();
			else Surface::EnableGrid();
			Surface::IsGridEnabled() ? std::cout << "[DEBUG] Grid ON!\n" : std::cout << "[DEBUG] Grid OFF!\n";
		}
	}
	if (KeyCode[GLFW_KEY_ESCAPE]) {
		if (areWindowsClosed()) {
			clearEditorVariables();
			engine::Engine::Reset();
		}
		else {
			clearEditorVariables();
			EDITOR_UI()->close_menu();
		}
	}
}

Editor::~Editor() { }