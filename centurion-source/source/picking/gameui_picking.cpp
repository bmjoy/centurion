#include <interface>
#include <picking.h>
#include <engine.h>
#include <game/strategy.h>

using namespace glb;


namespace game {
	void UIGame::picking() {
		Picking::leftClickID_UI = Picking::GetIdFromClick();
		string clickName = PickingUI::getPickedObjectName(Picking::leftClickID_UI);
		if (clickName == "GAME_pauseMenuButtonClose") {
			GameMenu::Disable();
		}
		if (clickName == "GAME_pauseMenuButtonQuit") {
			GameMenu::Disable();
			Engine::Reset();
		}
	}
};