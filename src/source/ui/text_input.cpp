#include <ui.h>
//#include <codecvt>
//#include <locale>
//
//#include <cwchar>
#include <engine.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace gui {

	TextInput::TextInput() {}
	
	void TextInput::create(string text, float x, float y, int maxChars) {
		xPos = x; yPos = y; max_chars = maxChars;
		
		static_text = gui::SimpleText("static");
		static_text.create_static(text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)text.size();

		for (int i = 0; i < 100; i++) currentText[i] = 0;
	}

	void TextInput::render() {
		if (is_active) {

			if (Engine::Keyboard::GetCharCodepointPressed() != -1 && cursorPosition <= max_chars) {
				currentText[cursorPosition] = Engine::Keyboard::GetCharCodepointPressed();
				cursorPosition++;
				static_text.create_static(currentText, cursorPosition + 1, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			if (Engine::Keyboard::IsKeyNotReleased(GLFW_KEY_BACKSPACE) && cursorPosition > 0) {
				currentText[cursorPosition - 1] = 0;
				cursorPosition--;
				static_text.create_static(currentText, cursorPosition + 1, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			text_cursor.render(vec4(255.f), false, 0, xPos + static_text.get_width(cursorPosition), yPos);
		}
		
		static_text.render_static();
	}

	std::string TextInput::get_text()
	{
		std::string text;
		for (int i = 0; i < 100; i++) {
			if (currentText[i] != 0) {
				text += (char)currentText[i];
			}
		}
		return text;
	}

	TextInput::~TextInput() {}
};
