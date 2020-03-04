#include <interface>
#include <picking>
#include <surface>
#include <global>
#include <engine/window.h>
#include <engine/mouse.h>

using namespace glb;
using namespace engine;

namespace editor {

	OpenMapWindow::OpenMapWindow() {
		back_color = glm::vec4(60.f, 68.f, 104.f, 255.f);
	}
	
	void OpenMapWindow::create() {
		back_image = gui::Image("openmapwindow_back");
		back_image.create("center", myWindow::Width / 2.f, myWindow::Height / 2.f, 0, 0, 0);
		availableScenarios = get_all_folders_names_within_folder("scenarios");
		map_list.padding_left = 15.f;
		map_list.padding_top = 15.f;
		map_list.option_height = 25.f;

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = myWindow::Width / 2.f - back_image.getImageSize().x / 2.f;
		startY = myWindow::Height / 2.f + back_image.getImageSize().y / 2.f;

		map_list.pickingID = getPickingID_UI();
		increasePickingID_UI();

		buttons[0] = gui::Image("openmapwindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "OpenMapWindow_close");
		increasePickingID_UI();

		buttons[1] = gui::Image("openmapwindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID_UI());
		addValueToPickingListUI(getPickingID_UI(), "OpenMapWindow_open");
		increasePickingID_UI();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_openMapButtonClose"), "tahoma_13px", startX + 58.f, startY - 468.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_openMapButtonOpen"), "tahoma_13px", startX + 314.f, startY - 468.f, "center", "middle", vec4(255.f), "bold");

		selectedID = -1;
	}

	void OpenMapWindow::update() {
		map_list.back_options.clear();
		map_list.text_options.clear();
		availableScenarios = get_all_folders_names_within_folder("scenarios");
		for (int i = 0; i < availableScenarios.size(); i++) {
			
			/* options texts */
			gui::SimpleText tempText = gui::SimpleText("static");
			string font = "tahoma_15px";
			float x = startX + map_list.padding_left;
			float y = startY - map_list.padding_top - (i + 1) * map_list.option_height;
			tempText.create_static(availableScenarios[i], font, x + 5.f, y + 5.f, "left", "normal", vec4(255.f));
			map_list.text_options.push_back(tempText);

			/* options rectangles */
			gui::Rectangle tempRect = gui::Rectangle();
			float w = tempText.get_width() + 10.f;
			tempRect.create("filled", x, y, w, map_list.option_height, "bottom-left", map_list.pickingID);
			map_list.back_options.push_back(tempRect);
		}
		OpenMapWindowUpdate = false;
		selectedID = -1;
	}

	void OpenMapWindow::render(bool pick) {

		if (OpenMapWindowUpdate) update();

		if (OpenMapWindowIsOpen) {
			if (pick && engine::Mouse::LeftClick) {
				for (int i = 0; i < availableScenarios.size(); i++) {
					map_list.back_options[i].render(vec4(), true);
				}
				buttons[0].render(true);
				buttons[1].render(true);
				picking();
			}
			if (!pick) {
				if (KeyCode[GLFW_KEY_UP]) {
					if (selectedID > 0) {
						selectedID--;
					}
				}
				if (KeyCode[GLFW_KEY_DOWN]) {
					if (selectedID < availableScenarios.size() - 1) {
						selectedID++;
					}
				}
				back_image.render(false);
				for (int i = 0; i < availableScenarios.size(); i++) {
					if (i == selectedID) {
						map_list.back_options[i].render(vec4(0, 0, 0, 0.5f));
					}
					map_list.text_options[i].render_static();					
				}
				buttons[0].render(false);
				buttons[1].render(false);
				buttons_text[0].render_static();
				buttons_text[1].render_static();
			}
		}
	}

	OpenMapWindow::~OpenMapWindow() {}
	
};
