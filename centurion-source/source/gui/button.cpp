#include "button.h"

Button::Button()
{
}

void Button::create() {

	buttonImage = Image(SHD::IMAGE_SHADER_ID);
	buttonImage.create("assets/ui/buttons/button_1.png", "center");

	buttonText = gui::SimpleText("static");
	buttonText.create_static(text, "tahoma_8", position.x, position.y, "center", "middle", glm::vec4(0.f, 0.f, 0.f, 255.f));
}

void Button::render(bool picking) {
	buttonImage.apply_projection_matrix(GLB::MENU_PROJECTION);
	if(!picking){
		buttonImage.render(position.x, position.y);
		buttonText.render_static();
	}
	else {
		buttonImage.render(position.x, position.y, true, picking_id);
	}
}

Button::~Button()
{
}
