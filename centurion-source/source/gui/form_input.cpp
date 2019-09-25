#include "form_input.h"

FormInput::FormInput(){
	boolOptions = true;
}

FormInput::FormInput(bool options){
	boolOptions = options;
}

void FormInput::create(float w, float h, std::vector<std::string> options) {
	
	width = w; height = h;
	form_options = options;
	isOpened = false;
	selectedText = options[0];	

	back = gui::Rectangle();
	back.set_id(picking_id);
	back.create("filled", position.x, position.y, width - 1, height - 1, "top-left");

	if (boolOptions) {
		for (int j = 0; j < options.size(); j++) {
			gui::Rectangle tempRect = gui::Rectangle();
			tempRect.set_id(picking_id);
			tempRect.set_color(color);
			float y = position.y - 1.f - (j + 1)*height + j * 1;
			tempRect.create("filled", position.x, y, width - 1, height - 1, "top-left");
			back_options.push_back(tempRect);
		}
	}
}

void FormInput::render(bool picking, glm::vec4 border_color) {

	obj::Text()->set_align();

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

		back.set_color(color);
		back.render();
		
		obj::Text()->render("tahoma_8", position.x + 3.f, (position.y - 1.f) - height / 1.5f - 6.f, selectedText);
		
		if (boolOptions && isOpened){
			for (int j = 0; j < form_options.size(); j++) {
				float y = position.y -1.f - (j + 1)*height + j*1;
				obj::ERectangle()->create(getCoords(position.x, position.y - height - 1.f, width, height * form_options.size() - 1.0f));
				obj::ERectangle()->render(glm::mat4(1.0f), glm::mat4(1.0f));
				
				back_options[j].render();
				
				obj::Text()->render("tahoma_8", position.x + 3.f, y - height / 1.5f - 6.f, form_options[j]);
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