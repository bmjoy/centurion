#include "form_input.h"

namespace gui {
	FormInput::FormInput(bool options) {
		boolOptions = options;
		hasText = true;
	}

	void FormInput::create(float xPos, float yPos, float w, float h, std::vector<std::string> options, int pickingID) {

		width = w; height = h; x = xPos; y = yPos;
		form_options = options;
		isOpened = false;
		selectedText = options[0];
		hasText = selectedText.size() > 0;
		mainTextPos = glm::vec2(x + 3.f, (y - 1.f) - height / 1.5f - 6.f);
		nOptions = options.size();

		back = gui::Rectangle();
		back.create("filled", x, y, width, height, "top-left", pickingID);

		if (boolOptions) {

			text = gui::SimpleText("dynamic"); // this is for selected text;

			back_options = gui::Rectangle();
			back_options.create("filled", x, y - height - 1, width, height * nOptions, "top-left", 0);

			for (int j = 0; j < options.size(); j++) {
				float y1 = y - 1.f - (j + 1)*height + j;

				/* options background saved in memory for picking */
				gui::Rectangle tempRect = gui::Rectangle();
				tempRect.create("filled", x, y1, width - 1, height - 1, "top-left", pickingID);
				back_options_picking.push_back(tempRect);

				/* options text saved in memory */
				gui::SimpleText tempText = gui::SimpleText("static");
				tempText.create_static(options[j], "tahoma_8", x + 3.f, y1 - height / 1.5f - 6.f, "left", "normal", glm::vec4(255.f));
				optionsText.push_back(tempText);
			}
		}
		else { // in this case the forminput cannot be opened
			if (hasText) {
				text = gui::SimpleText("static");
				text.create_static(options[0], "tahoma_8", mainTextPos.x, mainTextPos.y, "left", "normal", glm::vec4(255.f));
			}
		}
	}

	void FormInput::render(bool picking, glm::vec4 color) {

		if (picking) {
			back.render(glm::vec4(), true);
			if (boolOptions && isOpened) {
				for (int j = 0; j < nOptions; j++) {
					back_options_picking[j].render(glm::vec4(), true);
				}
			}
		}
		else {

			back.render(color, false);

			obj::ERectangle()->create(getCoords(x, y, width, height));
			obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f), glm::vec4(255.0f, 255.0f, 255.0f, 1.0f));

			if (boolOptions) {
				// selected text
				text.render_dynamic(selectedText, "tahoma_8", mainTextPos.x, mainTextPos.y, glm::vec4(255.f), "left", "normal");
				if (isOpened) {
					// background
					back_options.render(color, false);
					// border 
					obj::ERectangle()->create(getCoords(x, y - height, width, height * nOptions + 1.0f));
					obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f));
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

	void FormInput::open_close() {
		isOpened = !isOpened;
	}

	void FormInput::close() {
		isOpened = false;
	}

	void FormInput::select_option(int i) {
		isOpened = false;
		selectedText = form_options[i - 1];
	}

	FormInput::~FormInput() {}
}
