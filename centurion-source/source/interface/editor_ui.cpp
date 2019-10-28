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
		open_map_window.render(picking);
		new_map_window.render(picking);
		add_object_window.render(picking);
		properties_window.render(picking);
		terrainbrush_window.render(picking);
		Q_WINDOW()->render(picking);

		// minimap rectangle:
		if (game::gameMinimapStatus) {
			float x = getParam("camera-x-position") / game::mapWidth * getParam("window-width");
			float y = getParam("camera-y-position") / game::mapHeight * (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")) + getParam("ui-bottom-height");
			float w = getParam("window-width-zoomed") * getParam("window-width") / game::mapWidth;
			float h = getParam("window-height-zoomed") * (getParam("window-height") - getParam("ui-bottom-height") - getParam("ui-top-height")) / game::mapHeight;
			x = std::max(x, 1.f);
			y = std::max(y, getParam("ui-bottom-height") + 1.f);
			y = std::min(y, getParam("window-height") - getParam("ui-top-height") - h);
			minimapRectangle.render(vec4(255.f), false, x, y, w, h);
		}

		editor_menu.render(picking); // always the last
	}

	EditorUI::~EditorUI() {}
	
};
