#include <interface>
#include <picking>
#include <surface>
#include <global>
#include <engine/window.h>
#include <engine/mouse.h>

using namespace glb;
using namespace engine;

namespace editor {

	NewMapWindow::NewMapWindow() {}
	
	void NewMapWindow::create() {
		back_image = gui::Image("newmapwindow_back");
		back_image.create("center", myWindow::Width/2.f, myWindow::Height/2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = myWindow::Width / 2.f - back_image.getImageSize().x / 2.f;
		startY = myWindow::Height / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("newmapwindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "NewMapWindow_close");
		increasePickingID();

		buttons[1] = gui::Image("newmapwindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "NewMapWindow_create");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_newMapButtonClose"), "tahoma_13px", startX + 58.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_newMapButtonCreate"), "tahoma_13px", startX + 314.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");

		textinput_back = gui::Image("newmapwindow_textinput");
		textinput_back.create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "NewMapWindow_textclick");
		increasePickingID();

		text[0] = gui::SimpleText("static");
		text[0].create_static(getTranslation("EDITOR_newScenario"), "tahoma_15px", startX + 15.f, startY - 25.f, "left", "normal", vec4(255), "bold");
		text[1] = gui::SimpleText("static");
		text[1].create_static(getTranslation("EDITOR_newScenarioName"), "tahoma_15px", startX + 15.f, startY - 85.f, "left", "normal", vec4(255), "normal");

		currentMapName = getTranslation("EDITOR_defaultMapName");
		text_input.create(currentMapName, startX + 21.f, startY - 131.f, 30);
		
		// per default when you open the editor, the window is opened
		text_input.active();
		NewMapWindowIsOpen = true;
		IsWindowOpened = true; 
	}

	void NewMapWindow::render(bool pick) {
		if (NewMapResetText) {
			text_input.create(currentMapName, startX + 21.f, startY - 131.f, 30);
			NewMapResetText = false;
		}

		if (NewMapWindowIsOpen) {
			if (pick && engine::Mouse::LeftClick) {
				buttons[0].render(true);
				buttons[1].render(true);
				textinput_back.render(true);
				picking();
			}
			if (!pick) {
				back_image.render(false);
				buttons[0].render(false);
				buttons[1].render(false);
				buttons_text[0].render_static();
				buttons_text[1].render_static();				
				text[0].render_static();
				text[1].render_static();
				text_input.render();
			}
			currentMapName = text_input.get_text();
		}	
	}

	NewMapWindow::~NewMapWindow() {}
};
