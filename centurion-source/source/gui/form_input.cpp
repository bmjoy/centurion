#include "form_input.h"

FormInput::FormInput(bool options){
	boolOptions = options;
	hasText = true;
}

void FormInput::create(float w, float h, std::vector<std::string> options) {
	
	width = w; height = h;
	form_options = options;
	isOpened = false;
	selectedText = options[0];	
	hasText = selectedText.size() > 0;
	mainTextPos = glm::vec2(position.x + 3.f, (position.y - 1.f) - height / 1.5f - 6.f);

	back = gui::Rectangle();
	back.set_id(picking_id);
	back.create("filled", position.x, position.y, width - 1, height - 1, "top-left");

	if (boolOptions) { 

		text = gui::SimpleText("dynamic"); // this is for selected text;
		
		for (int j = 0; j < options.size(); j++) {			
			float y = position.y - 1.f - (j + 1)*height + j;
			
			/* options background saved in memory */
			
			gui::Rectangle tempRect = gui::Rectangle();
			tempRect.set_id(picking_id);
			tempRect.set_color(color);
			tempRect.create("filled", position.x, y, width - 1, height - 1, "top-left");
			back_options.push_back(tempRect);

			/* options text saved in memory */

			gui::SimpleText tempText = gui::SimpleText("static");
			tempText.create_static(options[j], "tahoma_8", position.x + 3.f, y - height / 1.5f - 6.f, "left", "normal", glm::vec4(255.f));
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

void FormInput::render(bool picking, glm::vec4 border_color) {

	if (picking) {
		back.render(true);
		if (boolOptions && isOpened) {
			for (int j = 0; j < back_options.size(); j++) {
				back_options[j].render(true);
			}
		}
	}
	else {

		obj::ERectangle()->create(getCoords(position.x, position.y, width, height));
		obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f), border_color);

		back.set_color(color); // this comes from ui object
		back.render();
		
		if (boolOptions) {
			text.set_text(selectedText);
			text.set_position(mainTextPos);
			text.render_dynamic("tahoma_8", glm::vec4(255.f), "left", "normal");
			if (isOpened) {
				for (int j = 0; j < form_options.size(); j++) {
					float y = position.y - 1.f - (j + 1)*height + j * 1;
					obj::ERectangle()->create(getCoords(position.x, position.y - height - 1.f, width, height * form_options.size() - 1.0f));
					obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f));

					back_options[j].render();
					optionsText[j].render_static();
				}
			}
		}
		else {
			if (hasText) {
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

FormInput::~FormInput()
{
}