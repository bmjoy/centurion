#include "editorUi.h"

#include "editorWindows.h"
#include "editorMenuBar.h"

#include <game/game.h>

using namespace std;
using namespace glm;

void EditorUI::Create()
{
	EditorMenuBar::Create();
	EditorWindows::Create();
}

void EditorUI::Render(bool picking)
{
	EditorWindows::Render(picking);
	if (Game::Minimap::IsActive() == false) {
		EditorMenuBar::Render(picking); // always the last
	}
}

EditorUI::~EditorUI() {}