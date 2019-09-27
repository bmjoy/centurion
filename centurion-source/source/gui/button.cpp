#include "button.h"

namespace gui {
	Button::Button(){
	}

	void Button::create(std::string imageName, int x, int y) {

		buttonImage = Image(imageName);
		buttonImage.set_id(picking_id);
		buttonImage.create("center", x, y);

		buttonText = gui::SimpleText("static");
		buttonText.create_static(text, "tahoma_8", x, y, "center", "middle", glm::vec4(0.f, 0.f, 0.f, 255.f));
	}

	void Button::render(bool picking) {
		buttonImage.render(picking);

		if (!picking) buttonText.render_static();
	}

	Button::~Button()
	{
	}

}
