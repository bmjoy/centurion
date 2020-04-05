#include <ui.h>
#include <engine.h>
#include <picking.h>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace gui {

	std::array<TextInput*, MAX_NUMBER_OF_TEXT_INPUTS> TextInput::TextInputs = { nullptr };

	TextInput::TextInput() {
		displayRectangle = true;
	}
	
	void TextInput::Create(const int _id, const int picking_id, const int x, const int y, const int width, const std::string font, const bool hasBackground, glm::vec4 backCol, const glm::vec4 borderCol, const std::string fontWeight, const std::wstring placeholderText) {
		xPos = (float)x; 
		yPos = (float)y; 
		max_chars = 100;
		id = _id;
		backColor = backCol;
		borderColor = borderCol;
		displayRectangle = hasBackground;

		static_text = gui::SimpleText("static");
		static_text.create_static(placeholderText, font, xPos, yPos, "left", "normal", vec4(255.f), fontWeight);

		float rectHeight = static_text.GetTextHeight();

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)placeholderText.size();

		rectangle = gui::Rectangle();
		rectangle.create("border-filled", xPos, yPos, (float)width, rectHeight, "bottom-left", picking_id);

		for (int i = 0; i < 100; i++) currentText[i] = 0;

		TextInput::AddTextInputToArray(id, this);
	}

	void TextInput::Render(bool picking) {

		if (picking)
		{
			rectangle.render(vec4(), vec4(), true);
		}
		else
		{
			if (Engine::Mouse::LeftClick)
			{
				if (rectangle.IsClicked(PickingUI::GetLeftClickId()))
				{
					this->Enable();
				}
				else
				{
					this->Disable();
				}
			}

			if (displayRectangle) rectangle.render(backColor, borderColor, false);

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

				text_cursor.render(vec4(255.f), vec4(), false, 0, xPos + static_text.get_width(cursorPosition) + 1, yPos + 1);
			}
			static_text.render_static();
		}
		
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

	void TextInput::Reset(void)
	{
		for (int i = 0; i < 100; i++) {
			if (currentText[i] != 0) {
				currentText[i] = 0;
			}
		}
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
