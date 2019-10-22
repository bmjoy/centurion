#include <interface>
#include <picking>
#include <engine>

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
	}

	void EditorUI::render(bool picking) {
		open_map_window.render(picking);
		new_map_window.render(picking);
		add_object_window.render(picking);

		editor_menu.render(picking); // always the last
	}

	EditorUI::~EditorUI() {}
	
};
