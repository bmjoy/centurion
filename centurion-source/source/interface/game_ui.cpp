#include <interface>
#include <engine.h>
#include <game/strategy.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;



namespace game {

	UIGame* GAME_UI() {
		return &myuigame;
	}

	UIGame::UIGame() { }

	void UIGame::create(string race) {

		time.seconds = 0; time.seconds_str = "00";
		time.minutes = 0; time.minutes_str = "00";
		time.hours = 0; time.hours_str = "00";
		time.x = Engine::myWindow::Width - 100.0f;
		time.y = Engine::myWindow::Height - Engine::myWindow::TopBarHeight - 30.0f;

		gameMenu = GameMenu();
		gameMenu.create();

		top_bar = gui::Image("topbar-" + race);
		top_bar.create("bottom-left", 0, -1.f * Engine::myWindow::TopBarHeight, Engine::myWindow::Width, Engine::myWindow::TopBarHeight, 0);
		

		bottom_bar = gui::Image("bottombar");
		bottom_bar.create("bottom-left", 0, 0, Engine::myWindow::Width, Engine::myWindow::BottomBarHeight, 0);

		time.lastTime = glfwGetTime();
		time.text = gui::SimpleText("dynamic", true);

		minimapRectangle = gui::Rectangle();
		minimapRectangle.create("border", 0, 0, 0, 0, "bottom-left", 0);

		objectUI = ObjectUI();
		objectUI.create("No item selected");

		set_ui(nullptr);
	}

	void UIGame::set_ui(ObjectUI* objUI) {
		if (objUI == nullptr) {
			objectUIptr = &objectUI;
		}
		else {
			objectUIptr = objUI;
		}
	}

	void UIGame::render(bool pick) {

		if (pick && Engine::Mouse::LeftClick) {
			if (GameMenu::IsActive()) gameMenu.render(true);
			picking(); // --> source/picking/gameui_picking.cpp  
		}

		if (!pick) {
			if (GameMenu::IsActive()) { gameMenu.render(false); }

			calculateTime();
			string txt = time.hours_str + ":" + time.minutes_str + ":" + time.seconds_str;
			time.text.render_dynamic(txt, "tahoma_15px", time.x, time.y, vec4(255.f, 255.f, 255.f, 255.f), "left", "normal");

			// minimap rectangle:
			if (Game::Minimap::IsActive()) {
				float x = Engine::Camera::GetXPosition() / MEDIUM_MAP_WIDTH * Engine::myWindow::Width;
				float y = Engine::Camera::GetYPosition() / MEDIUM_MAP_HEIGHT * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) + Engine::myWindow::BottomBarHeight;
				float w = Engine::myWindow::WidthZoomed * Engine::myWindow::Width / MEDIUM_MAP_WIDTH;
				float h = Engine::myWindow::HeightZoomed * (Engine::myWindow::Height - Engine::myWindow::BottomBarHeight - Engine::myWindow::TopBarHeight) / MEDIUM_MAP_HEIGHT;
				x = std::max(x, 1.f);
				y = std::max(y, Engine::myWindow::BottomBarHeight + 1.f);
				y = std::min(y, Engine::myWindow::Height - Engine::myWindow::TopBarHeight - h);
				minimapRectangle.render(vec4(255.f), false, x, y, w, h);
			}
			// Temporary bars:
			top_bar.render(false, 0, 0, true);
			bottom_bar.render(false, 0, 0, true);

			objectUIptr->render();
		}

	}

	void UIGame::calculateTime() {
		if (glfwGetTime() - time.lastTime >= 1.0f) {
			time.seconds++;
			time.lastTime += 1.0f;
		}
		if (time.seconds == 60) {
			time.seconds = 0;
			time.minutes++;
		}
		if (time.minutes == 60) {
			time.minutes = 0;
			time.hours++;
		}
		if (time.seconds < 10) { time.seconds_str = "0" + to_string(time.seconds); }
		else { time.seconds_str = to_string(time.seconds); }
		if (time.minutes < 10) { time.minutes_str = "0" + to_string(time.minutes); }
		else { time.minutes_str = to_string(time.minutes); }
		if (time.hours < 10) { time.hours_str = "0" + to_string(time.hours); }
		else { time.hours_str = to_string(time.hours); }
	}

	UIGame::~UIGame() {}
};
