#include "button.h"

Button::Button()
{
}

void Button::create() {

	buttonImage = Image(SHD::IMAGE_SHADER_ID);
	buttonImage.create("assets/ui/buttons/button_1.png", "center");
	buttonImage.apply_projection_matrix(GLB::MENU_PROJECTION);

	buttonText = CBitmapFont(SHD::FONT_SHADER_ID);
	buttonText.apply_projection_matrix(GLB::MENU_PROJECTION);
	buttonText.set_align("center", "middle");
	buttonText.create("tahoma_8");
}

void Button::render(bool picking) {

	if(!picking){
		buttonImage.render(position.x, position.y);
		buttonText.render(position.x, position.y, text, glm::vec4(0.f,0.f,0.f,255.f));
		
	}
	else {
		buttonImage.render(position.x, position.y, true, picking_id);
	}
}

Button::~Button()
{
}
