#include <interface>
#include <picking>
#include <engine>
#include <game>

using namespace glb;
using namespace engine;

namespace game {
	void UIGame::picking() {
		leftClickID_UI = get_id();
		string clickName = getPickedObjectName(leftClickID_UI);
		if (clickName == "GAME_pauseMenuButtonClose") {
			gameMenuStatus = false;
		}
		if (clickName == "GAME_pauseMenuButtonQuit") {
			gameMenuStatus = false;
			//saveCurrentScenario("random_map_singleplayer");
			ENGINE()->Reset();
		}
	}
};