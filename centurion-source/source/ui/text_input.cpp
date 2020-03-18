#include <ui.h>
#include <codecvt>
#include <locale>
//
#include <engine.h>

#include <GLFW/glfw3.h>

namespace gui {

	TextInput::TextInput() {}
	
	void TextInput::create(string text, float x, float y, int maxChars) {
		xPos = x; yPos = y; max_chars = maxChars;
		
		static_text = gui::SimpleText("static");
		static_text.create_static(text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
		current_text = text;

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)text.size();
	}

	void TextInput::render() {
		if (is_active) {

			if (Engine::Keyboard::GetCharCodepointPressed() != -1 && current_text.size() <= max_chars) {
				wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
				wstring wchar = L" ";
				wchar[0] = Engine::Keyboard::GetCharCodepointPressed();
				string c = converter.to_bytes(wchar);
				current_text += c;
				cursorPosition++;
				static_text.create_static(current_text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			if (Engine::Keyboard::IsKeyPressed(GLFW_KEY_BACKSPACE) && current_text.size() > 0) {
				current_text.erase(current_text.end() - 1);
				cursorPosition--;
				static_text.create_static(current_text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			text_cursor.render(vec4(255.f), false, 0, xPos + static_text.get_width(cursorPosition), yPos);
		}
		
		static_text.render_static();
	}

	TextInput::~TextInput() {}
};
