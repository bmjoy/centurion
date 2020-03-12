#include <game/editor.h>
#include <surface>
#include <game/strategy.h>
#include <picking.h>
#include <interface>
#include <engine.h>

#include <global>

using namespace glb;


#pragma region Static variables

#pragma endregion


Editor::Editor() {}

void Editor::Create() {
	PickingUI::resetPicking();
	PickingObject::resetPicking();

	Strategy::reset();
	Engine::myWindow::BottomBarHeight = 0.f;
	Engine::myWindow::TopBarHeight = 30.f;
	setMinimapProjection();

	Surface::Reset();
	editor::EDITOR_UI()->create();

	SelectionRectangle::Create();

	Engine::Mouse::LeftHold = false;

	Engine::Camera::GoToPoint(1.f, 1.f);

	isCreated = true;
	Minimap::Update();
}

void Editor::Run() {
	/* Keyboard control */
	handleKeyboardControls();
	if (!editor::IsWindowOpened) { // TODO: merge all these in a function in Editor->Editor_functions.cpp
		Engine::Camera::keyboardControl();
	}

	/* If minimap is NOT active */
	if (Minimap::IsActive() == false) {
		if (!editor::IsWindowOpened && Engine::Mouse::GetYPosition() < Engine::myWindow::Height - 30.f && !editor::menuIsOpened)
			Engine::Camera::mouseControl();
		viewMatrix = Engine::Camera::calculateViewMatrix();
		projectionMatrix = glb::cameraProjection;

		editor::EDITOR_UI()->render(true);

		// apply game matrices
		applyGameMatrices(&projectionMatrix, &viewMatrix);

		// picking
		if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) RenderObjectsPicking();

		// rendering
		Surface::Render(false);
		RenderObjects();
		if (!editor::IsWindowOpened && !editor::addingObject && !editor::TerrainBrushIsActive) editor::moveObjects();

		// apply menu matrices
		applyMenuMatrices();

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
			applyGameMatrices(&projectionMatrix, &viewMatrix);
			Surface::Render(false);
			RenderObjects();
			Minimap::Create();
			applyMenuMatrices();
		}

		editor::EDITOR_UI()->render(false);

		if (Picking::leftClickID_UI == 0) GoToPointFromMinimap();
	}

	glb::cameraProjection = glm::ortho(0.0f, Engine::myWindow::WidthZoomed, 0.0f, Engine::myWindow::HeightZoomed, -(float)MEDIUM_MAP_WIDTH, (float)MEDIUM_MAP_WIDTH);

	Engine::Mouse::RightClick = false;
	Engine::Mouse::LeftClick = false;
	Engine::Mouse::MiddleClick = false;
	Engine::Keyboard::SetKeyStatus(GLFW_KEY_ESCAPE, false);

	if (editor::IsWindowOpened) {
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_BACKSPACE, false);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_DELETE, false);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_UP, false);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_DOWN, false);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_LEFT, false);
		Engine::Keyboard::SetKeyStatus(GLFW_KEY_RIGHT, false);
	}
}



void Editor::handleKeyboardControls() {

	using namespace editor;


	//CTRL Hotkeys
	if (!IsWindowOpened) {
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || Engine::Keyboard::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL)) {
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_N)) { NewMapWindowIsOpen = true; NewMapResetText = true; IsWindowOpened = true; }
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_O)) { OpenMapWindowIsOpen = true; OpenMapWindowUpdate = true; IsWindowOpened = true; }
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_S)) { saveCurrentScenario(currentMapName); }
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_A)) { TerrainBrushIsActive = false; TerrainBrushWindowIsOpen = false; AddObjectWindowIsOpen = !AddObjectWindowIsOpen; }
			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_T)) { AddObjectWindowIsOpen = false; TerrainBrushIsActive = !TerrainBrushWindowIsOpen; TerrainBrushWindowIsOpen = !TerrainBrushWindowIsOpen; }
		}
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_DELETE)){
			if (Game::IsGameObjectNotNull(Picking::leftClickID)) {
				Building* b = Game::GetGameObjectPtrById(Picking::leftClickID)->AsBuilding();
				if (b->isSelected()) {
					if (b->is_independent()) {
						if (b->buildingsInSettlementCount() > 0) {
							b->setWaitingToBeErased(true);
							Q_WINDOW()->setQuestion("QUESTION_deleteAll");
						}
						else {
							cout << "[DEBUG] Settlement " << b->get_name() << " deleted!\n";
							b->clear_pass();
							Game::RemoveGameObject(Picking::leftClickID);
						}
					}
					else {
						cout << "[DEBUG] Building " << b->get_name() << " deleted!\n";
						b->clear_pass();
						Game::RemoveGameObject(Picking::leftClickID);
					}
				}
			}
		}
		
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_SPACE) || Engine::Mouse::MiddleClick) {
			if (Minimap::IsActive()) Minimap::Disable();
			else Minimap::Enable();
			Minimap::IsActive() ? Logger::Info("Minimap ON!") : Logger::Info("Minimap OFF!");
		}
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_Z)) {
			Surface::Wireframe = !Surface::Wireframe;
			Surface::Wireframe ? Logger::Info("Wireframe ON!") : Logger::Info("Wireframe OFF!");
		}
		// Grid
		if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_G)) {
			if (Surface::IsGridEnabled()) Surface::DisableGrid();
			else Surface::EnableGrid();
			Surface::IsGridEnabled() ? Logger::Info("Grid ON!") : Logger::Info("Grid OFF!");
		}
	}
	if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_ESCAPE)) {
		if (areWindowsClosed()) {
			clearEditorVariables();
			Engine::Reset();
		}
		else {
			clearEditorVariables();
			EDITOR_UI()->close_menu();
		}
	}
}

Editor::~Editor() { }