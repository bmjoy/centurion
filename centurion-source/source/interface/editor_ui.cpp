#include <interface>
#include <game/strategy.h>
#include <engine.h>

#include <game/interface/editorMenuBar.h>



namespace editor {

	EditorUI *EDITOR_UI() { return &myeditorUI; }

	EditorUI::EditorUI() {}

	void EditorUI::create() {
		
		EditorMenuBar::Create();
		open_map_window.create();
		new_map_window.create();
		add_object_window.create();
		properties_window.create();
		terrainbrush_window.create();
		Q_WINDOW()->create();

		minimapRectangle = gui::Rectangle();
		minimapRectangle.create("border", 0, 0, 0, 0, "bottom-left", 0);
	}

	void EditorUI::render(bool picking) {
		// minimap rectangle:
		if (Game::Minimap::IsActive()) {
			float x = Engine::Camera::GetXPosition() / MEDIUM_MAP_WIDTH * Engine::myWindow::Width;
			float y = Engine::Camera::GetYPosition() / MEDIUM_MAP_HEIGHT * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) + Engine::myWindow::BottomBarHeight;
			float w = Engine::myWindow::WidthZoomed * Engine::myWindow::Width / MEDIUM_MAP_WIDTH;
			float h = Engine::myWindow::HeightZoomed * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) / MEDIUM_MAP_HEIGHT;
			x = std::max(x, 1.f);
			y = std::max(y, Engine::myWindow::BottomBarHeight + 1.f);
			y = std::min(y, Engine::myWindow::Height - Engine::myWindow::TopBarHeight - h);
			minimapRectangle.render(vec4(255.f), false, x, y, w, h);
		}

		open_map_window.render(picking);
		new_map_window.render(picking);
		add_object_window.render(picking);
		properties_window.render(picking);
		terrainbrush_window.render(picking);
		Q_WINDOW()->render(picking);

		EditorMenuBar::Render(picking); // always the last
	}

	EditorUI::~EditorUI() {}
	
};
