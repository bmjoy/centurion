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

	coords = getCoords(position.x, position.y, width, height);
	border = EmptyRectangle(SHD::E_RECTANGLE_SHADER_ID);
	border.init();
	border.create(coords);
	border.apply_projection_matrix(GLB::MENU_PROJECTION);

	coords = getCoords(position.x, position.y - height - 1.f, width, height * form_options.size() - 1.0f);
	optionsBorder = EmptyRectangle(SHD::E_RECTANGLE_SHADER_ID);
	optionsBorder.init();
	optionsBorder.create(coords);
	optionsBorder.apply_projection_matrix(GLB::MENU_PROJECTION);


	back = FilledRectangle(SHD::F_RECTANGLE_SHADER_ID);
	back.create(width-1, height-1, "top-left");
	back.apply_projection_matrix(GLB::MENU_PROJECTION);
	back.apply_view_matrix();

	text = CBitmapFont(SHD::FONT_SHADER_ID);
	text.apply_projection_matrix(GLB::MENU_PROJECTION);
	text.create("tahoma_8");
	
}

void FormInput::render(bool picking, glm::vec4 border_color) {
	if (picking) {
		back.render(position.x, position.y, true, picking_id);

		if (boolOptions && isOpened) {
			for (int j = 0; j < form_options.size(); j++) {
				float y = position.y - 1.f - (j + 1)*height + j * 1;
				back.render(position.x, y, true, picking_id);
			}
		}
	}
	else {
		back_color = color; // this comes from uiobject
		border.render(glm::mat4(1.0f), glm::mat4(1.0f), border_color);
		back.set_color(back_color);
		back.render(position.x, position.y-1.f);
		text.render(position.x + 3.f, (position.y - 1.f) - height / 1.5f - 6.f, selectedText);

		if (boolOptions && isOpened){
			for (int j = 0; j < form_options.size(); j++) {
				float y = position.y -1.f - (j + 1)*height + j*1;
				optionsBorder.render(glm::mat4(1.0f), glm::mat4(1.0f));
				back.set_color(back_color);
				back.render(position.x, y);
				text.render(position.x + 3.f, y - height / 1.5f - 6.f, form_options[j]);
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