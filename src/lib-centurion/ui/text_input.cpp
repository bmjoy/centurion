#include <ui.h>
#include <engine.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace gui {

	std::array<TextInput*, MAX_NUMBER_OF_TEXT_INPUTS> TextInput::TextInputs = { nullptr };

	TextInput::TextInput() {}
	
	void TextInput::Create(const int _id, float x, float y, int maxChars, wstring placeholderText) {
		xPos = x; 
		yPos = y; 
		max_chars = maxChars;
		id = _id;

		static_text = gui::SimpleText("static");
		static_text.create_static(placeholderText, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)placeholderText.size();

		for (int i = 0; i < 100; i++) currentText[i] = 0;

		TextInput::AddTextInputToArray(id, this);
	}

	void TextInput::Render() {
		if (isActive) {

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

	std::string TextInput::GetText(void)
	{
		std::string text;
		for (int i = 0; i < 100; i++) {
			if (currentText[i] != 0) {
				text += (char)currentText[i];
			}
		}
		return text;
	}

	void TextInput::AddTextInputToArray(int id, TextInput * txtInput)
	{
		if (id < 0 || id > MAX_NUMBER_OF_TEXT_INPUTS) return;
		TextInputs[id] = txtInput;
	}

	TextInput * TextInput::GetTextInputById(int id)
	{
		if (id < 0 || id > MAX_NUMBER_OF_TEXT_INPUTS) return nullptr;
		return TextInputs[id];
	}

	TextInput::~TextInput() {}
};
