#include "button.h"

Button::Button()
{
}

void Button::create() {

	buttonImage = Image(SHD::IMAGE_SHADER_ID);
	buttonImage.create("assets/ui/buttons/button_1.png", "center");

	buttonText = DivText();
}

void Button::render(bool picking) {
	buttonImage.apply_projection_matrix(GLB::MENU_PROJECTION);
	if(!picking){
		buttonImage.render(position.x, position.y);
		buttonText.set_position(position);
		buttonText.set_text(text);
		buttonText.render("tahoma_8", glm::vec4(0.f, 0.f, 0.f, 255.f), "center", "middle");
	}
	else {
		buttonImage.render(position.x, position.y, true, picking_id);
	}
}

Button::~Button()
{
}
