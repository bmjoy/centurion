#include <gui>
#include <codecvt>
#include <locale>

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	TextInput::TextInput() {}
	
	void TextInput::create(string text, float x, float y, int maxChars) {
		xPos = x; yPos = y; max_chars = maxChars;
		
		current_text = "prova";
		static_text = gui::SimpleText("static");
		static_text.create_static(text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
		current_text = text;

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)text.size();
	}

	void TextInput::render() {
		if (is_active) {

			if (CharCodepointPressed != -1 && current_text.size() <= max_chars){
				wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
				wstring wchar = L" ";
				wchar[0] = CharCodepointPressed;
				string c = converter.to_bytes(wchar);
				current_text += c;
				cursorPosition++;
				static_text.create_static(current_text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			if (KeyCode[GLFW_KEY_BACKSPACE] && current_text.size() > 0) {
				current_text.erase(current_text.end() - 1);
				cursorPosition--;
				static_text.create_static(current_text, "tahoma_15px", xPos, yPos, "left", "normal", vec4(255.f));
			}

			text_cursor.render(vec4(255.f), false, xPos + static_text.get_width(cursorPosition), yPos);
		}
		
		static_text.render_static();
	}

	TextInput::~TextInput() {}
};
