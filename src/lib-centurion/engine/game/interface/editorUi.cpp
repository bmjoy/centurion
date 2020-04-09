#include "editorUi.h"

#include "editorWindows.h"
#include "editorMenuBar.h"

#include <game/game.h>
#include <engine.h>

using namespace std;
using namespace glm;

namespace EditorUI
{
	namespace
	{
		gui::SimpleText infoText = gui::SimpleText("static");
	};

	void EditorUI::Create()
	{
		EditorMenuBar::Create();
		EditorWindows::Create();
		infoText.create_static("", "tahoma_15px", 10.f, -40.f, "left", "middle", glm::vec4(255.f));
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
			infoText.render_static();
		}
	}
	void UpdateInfoText(std::string infotext)
	{
		infoText.SetNewText(infotext);
	}
};


