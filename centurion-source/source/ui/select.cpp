#include <ui.h>

#include <engine.h>

#include <translationsTable.h>

namespace gui {

	using namespace std;
	using namespace glm;
	

	Select::Select(bool options) {
		boolOptions = options;
		hasText = true;
	}

	void Select::create(float xPos, float yPos, float w, float h, vector<string> options, int pickingID) {

		width = w; height = h; x = xPos; y = yPos;
		form_options = options;
		isOpened = false;
		selectedTextID = 0;
		hasText = form_options[selectedTextID].size() > 0;
		mainTextPos = vec2(x + 2.f, (y - 1.f) - height / 1.5f - 5.f);
		nOptions = (int)options.size();

		back = gui::Rectangle();
		back.create("border-filled", x, y, width, height, "top-left", pickingID);

		optionsText = {};
		back_options_picking = {};

		if (boolOptions) {

			text = gui::SimpleText("dynamic"); // this is for selected text;

			back_options = gui::Rectangle();
			back_options.create("border-filled", x, y - height - 1, width, height * nOptions, "top-left", 0);

			for (int j = 0; j < nOptions; j++) {
				float y1 = y - 1.f - (j + 1)*height + j;

				/* options background saved in memory for picking */
				gui::Rectangle tempRect = gui::Rectangle();
				tempRect.create("filled", x, y1, width - 1, height - 1, "top-left", pickingID);
				back_options_picking.push_back(tempRect);

				/* options text saved in memory */
				gui::SimpleText tempText = gui::SimpleText("static");
				tempText.create_static(TranslationsTable::GetTranslation(form_options[j]), "tahoma_15px", x + 2.f, y1 - height / 1.5f - 5.f, "left", "normal", vec4(255.f));
				optionsText.push_back(tempText);
			}
		}
		else { // in this case the forminput cannot be opened
			if (hasText) {
				text = gui::SimpleText("static");
	
				text.create_static(TranslationsTable::GetTranslation(form_options[selectedTextID]), "tahoma_15px", mainTextPos.x, mainTextPos.y, "left", "normal", vec4(255.f));
			}
		}
		selectedText = form_options[selectedTextID];
	}
	void Select::render(bool picking, vec4 color) {

		if (picking) {
			back.render(vec4(), true);
			if (boolOptions && isOpened) {
				for (int j = 0; j < nOptions; j++) {
					back_options_picking[j].render(color, true);
				}
			}
		}
		else {

			back.render(color, false);

			if (boolOptions) {
				// selected text
				text.render_dynamic(TranslationsTable::GetTranslation(selectedText), "tahoma_15px", mainTextPos.x, mainTextPos.y, vec4(255.f), "left", "normal");
				if (isOpened) {

					// background and border
					back_options.render(color, false);

					// text
					for (int j = 0; j < nOptions; j++) {
						optionsText[j].render_static();
					}

				}
			}
			else {
				if (hasText) {
					// selected text
					text.render_static();
				}
			}
		}
	}

	void Select::open_close() { isOpened = !isOpened; }

	void Select::close() { isOpened = false;	}

	void Select::select_option(int i) {
		isOpened = false;
		selectedTextID = i - 1;
		selectedText = form_options[selectedTextID];
	}

	int Select::get_clicked_option() {
		int i = int((Engine::Mouse::GetYLeftClick() - y) / height)*(-1);
		return i;
	}
	void Select::select_next() {
		if (selectedTextID < form_options.size() - 1) selectedTextID++;
		else selectedTextID = 0;
		selectedText = form_options[selectedTextID];
	}
	void Select::select_previous() {
		if (selectedTextID > 0) selectedTextID--;
		else selectedTextID = (int)form_options.size() - 1;
		selectedText = form_options[selectedTextID];
	}

	Select::~Select() {
	}
};
