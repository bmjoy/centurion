#include <interface>
#include <picking>
#include <engine>
#include <surface>

using namespace glb;
using namespace engine;


namespace editor {

	AddObjectWindow::AddObjectWindow() {}
	
	void AddObjectWindow::create() {

		back_size.x = 790; back_size.y = 260;

		back_image = gui::Image("addobjectwindow_back");
		back_image.create("center", getParam("window-width")/2.f, back_size.y / 2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = getParam("window-width") / 2.f - back_image.getImageSize().x / 2.f;
		startY = back_size.y / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("addobjectwindow_button1");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_add");
		increasePickingID();

		buttons[1] = gui::Image("addobjectwindow_button2");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_close");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("EDITOR_newMapButtonClose"), "tahoma_13px", startX + 725.f, startY - 28.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("EDITOR_newMapButtonCreate"), "tahoma_13px", startX + 725.f, startY - 233.f, "center", "middle", vec4(255.f), "bold");

		arrows[0] = gui::Image("addobjectwindow_arrowleft");
		arrows[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_leftarrow");
		increasePickingID();

		arrows[1] = gui::Image("addobjectwindow_arrowright");
		arrows[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "AddObjWindow_rightarrow");
		increasePickingID();


		IsWindowOpened = true; 
	}

	void AddObjectWindow::render(bool pick) {

		if (NewMapWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				buttons[0].render(true);
				buttons[1].render(true);
				arrows[0].render(true);
				arrows[1].render(true);				
				picking();
			}
			if (!pick) {
				back_image.render(false);
				buttons[0].render(false);
				buttons[1].render(false);
				arrows[0].render(false);
				arrows[1].render(false);
				buttons_text[0].render_static();
				buttons_text[1].render_static();				
			}
		}	
	}

	void AddObjectWindow::picking() {

	}

	AddObjectWindow::~AddObjectWindow() {}
};
