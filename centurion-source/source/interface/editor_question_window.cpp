#include <interface>
#include <picking.h>
#include <surface>
#include <game/editor.h>
#include <engine.h>
#include <translationsTable.h>




namespace editor {
	QuestionWindow::QuestionWindow() {}

	void QuestionWindow::create() {
		back_image = gui::Image("yesornotwindow_back");
		back_image.create("center", Engine::myWindow::Width / 2.f, Engine::myWindow::Height / 2.f, 0, 0, PickingUI::obtainPickingID());

		// startX and startY are TOP-LEFT coordinates (as in Paint)
		startX = Engine::myWindow::Width / 2.f - back_image.getImageSize().x / 2.f;
		startY = Engine::myWindow::Height / 2.f + back_image.getImageSize().y / 2.f;

		buttons[0] = gui::Image("newmapwindow_buttonleft");
		buttons[0].create("top-left", startX, startY, 0, 0, PickingUI::obtainPickingID());
		PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "QuestionWindow_Yes");

		buttons[1] = gui::Image("newmapwindow_buttonright");
		buttons[1].create("top-left", startX, startY, 0, 0, PickingUI::obtainPickingID());
		PickingUI::addValueToPickingList(PickingUI::getLastID() + 1, "QuestionWindow_No");

		buttons_text[0] = gui::SimpleText("static");
		buttons_text[0].create_static(TranslationsTable::GetTranslation("WORD_yes"), "tahoma_13px", startX + 58.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");
		buttons_text[1] = gui::SimpleText("static");
		buttons_text[1].create_static(TranslationsTable::GetTranslation("WORD_no"), "tahoma_13px", startX + 314.f, startY - 171.f, "center", "middle", vec4(255.f), "bold");
		
		questionText = gui::TextBox();
		questionText.create(TranslationsTable::GetTranslation(""), "tahoma_15px", startX + 15.f, startY - 85.f, back_image.getImageSize().x - 30.f, back_image.getImageSize().y, "left", "normal", vec4(255), "normal");
	}

	void QuestionWindow::render(bool pick) {
		if (QuestionWindowIsOpen) {
			if (pick && Engine::Mouse::LeftClick) {
				back_image.render(true);
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
				questionText.render();
			}
		}
	}

	void QuestionWindow::setQuestion(string Question) {
		Engine::Mouse::LeftClick = false;
		QuestionWindowIsOpen = true;
		IsWindowOpened = true;
		question = Question;
		questionText.create(TranslationsTable::GetTranslation(Question), "tahoma_15px", startX + 15.f, startY - 85.f, back_image.getImageSize().x - 30.f, back_image.getImageSize().y, "left", "normal", vec4(255), "normal");
	}

	QuestionWindow::~QuestionWindow() {}
};
