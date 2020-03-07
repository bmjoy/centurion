#include <interface>
#include <picking>
#include <game/strategy.h>
#include <engine/camera.h>
#include <engine/window.h>

using namespace glb;

using namespace engine;

namespace editor {

	EditorUI *EDITOR_UI() { return &myeditorUI; }

	EditorUI::EditorUI() {}

	void EditorUI::create() {
		editor_menu.create();
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
			float x = Camera::GetXPosition() / MEDIUM_MAP_WIDTH * myWindow::Width;
			float y = Camera::GetYPosition() / MEDIUM_MAP_HEIGHT * (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight) + myWindow::BottomBarHeight;
			float w = myWindow::WidthZoomed * myWindow::Width / MEDIUM_MAP_WIDTH;
			float h = myWindow::HeightZoomed * (myWindow::Height - myWindow::BottomBarHeight - myWindow::TopBarHeight) / MEDIUM_MAP_HEIGHT;
			x = std::max(x, 1.f);
			y = std::max(y, myWindow::BottomBarHeight + 1.f);
			y = std::min(y, myWindow::Height - myWindow::TopBarHeight - h);
			minimapRectangle.render(vec4(255.f), false, x, y, w, h);
		}

		open_map_window.render(picking);
		new_map_window.render(picking);
		add_object_window.render(picking);
		properties_window.render(picking);
		terrainbrush_window.render(picking);
		Q_WINDOW()->render(picking);

		editor_menu.render(picking); // always the last
	}

	EditorUI::~EditorUI() {}
	
};
