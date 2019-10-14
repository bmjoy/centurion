#include <interface>
#include <picking>
#include <engine>
#include <game>

using namespace glb;
using namespace engine;

namespace game {
	void UIGame::picking() {
		int clickId = get_id();
		string clickName = getPickedObjectName(clickId);
		if (clickName == "GAME_pauseMenuButtonClose") {
			gameMenuStatus = false;
		}
		if (clickName == "GAME_pauseMenuButtonQuit") {
			gameMenuStatus = false;
			ENGINE()->Reset();
		}
	}
};