#include "button.h"

namespace gui {
	Button::Button(){
	}

	void Button::create(std::string imageName, std::string Text, int x, int y, int pickingID, glm::vec4 textColor) {

		buttonImage = Image(imageName);
		buttonImage.create("center", x, y, 0, 0, pickingID);

		buttonText = gui::SimpleText("static");
		buttonText.create_static(Text, "tahoma_8", x, y, "center", "middle", textColor);
	}

	void Button::render(bool picking) {
		buttonImage.render(picking);

		if (!picking) buttonText.render_static();
	}

	Button::~Button()
	{
	}

}
