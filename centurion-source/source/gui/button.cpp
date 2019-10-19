#include <gui>

namespace gui {

	using namespace std;
	using namespace glm;

	Button::Button(){}

	void Button::create(string imageName, string Text, int x, int y, int pickingID, vec4 textColor) {
		buttonImage = Image(imageName);
		buttonImage.create("center", (float)x, (float)y, 0, 0, pickingID);
		buttonText = SimpleText("static");
		buttonText.create_static(Text, "tahoma_13px", (float)x, (float)y, "center", "middle", textColor, "bold");
	}
	void Button::render(bool picking) {
		buttonImage.render(picking);
		if (!picking) buttonText.render_static();
	}

	Button::~Button(){}
};