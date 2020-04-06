#include "editorUi.h"

#include "editorWindows.h"
#include "editorMenuBar.h"

#include <game/game.h>
#include <engine.h>

using namespace std;
using namespace glm;

void EditorUI::Create()
{
	EditorMenuBar::Create();
	EditorWindows::Create();
}

void EditorUI::Render(bool picking)
{
	if (picking)
	{
		if (Engine::Mouse::LeftClick || Engine::Mouse::RightClick)
		{
			EditorWindows::Render(true);
			EditorMenuBar::Render(true); 
		}
	}
	else
	{
		EditorWindows::Render(false);
		EditorMenuBar::Render(false);
	}
}
