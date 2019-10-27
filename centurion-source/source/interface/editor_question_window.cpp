#include <interface>
#include <picking>
#include <engine>
#include <surface>
#include <editor>

using namespace glb;
using namespace engine;


namespace editor {

	QuestionWindow::QuestionWindow() {}
	
	void QuestionWindow::create() {
		answer = -1;
		back_image = gui::Image("yesornotwindow_back");
		back_image.create("center", getParam("window-width") / 2.f, getParam("window-height") / 2.f, 0, 0, 0);

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = getParam("window-width") / 2.f - back_image.getImageSize().x / 2.f;
		startY = getParam("window-height") / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("newmapwindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "QuestionWindow_Yes");
		increasePickingID();

		buttons[1] = gui::Image("newmapwindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, getPickingID());
		addValueToPickingListUI(getPickingID(), "QuestionWindow_No");
		increasePickingID();

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(getTranslation("WORD_yes"), "tahoma_13px", startX + 58.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(getTranslation("WORD_no"), "tahoma_13px", startX + 314.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");
		questionText = gui::SimpleText("dynamic");
	}

	void QuestionWindow::render(bool pick) {
		if (QuestionWindowIsOpen) {
			if (pick && getBoolean("mouse-left")) {
				buttons[0].render(true);
				buttons[1].render(true);
				picking();
			}
			if (!pick) {
				back_image.render(false);
				buttons[0].render(false);
				buttons[1].render(false);
				buttons_text[0].render_static();
				buttons_text[1].render_static();
				questionText.render_dynamic(getTranslation(question), "tahoma_15px", startX + 15.f, startY - 85.f, vec4(255), "left", "normal");
			}
		}
	}

	QuestionWindow::~QuestionWindow() {}
};
