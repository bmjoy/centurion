#include <interface>
#include <picking>
#include <engine>
#include <game>

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
		if (game::gameMinimapStatus) {
			float x = getParam("camera-x-position") / game::mapWidth * settings.GetWindowWidth();
			float y = getParam("camera-y-position") / game::mapHeight * (settings.GetWindowHeight() - getParam("ui-bottom-height") - getParam("ui-top-height")) + getParam("ui-bottom-height");
			float w = getParam("window-width-zoomed") * settings.GetWindowWidth() / game::mapWidth;
			float h = getParam("window-height-zoomed") * (settings.GetWindowHeight() - getParam("ui-bottom-height") - getParam("ui-top-height")) / game::mapHeight;
			x = std::max(x, 1.f);
			y = std::max(y, getParam("ui-bottom-height") + 1.f);
			y = std::min(y, settings.GetWindowHeight() - getParam("ui-top-height") - h);
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
