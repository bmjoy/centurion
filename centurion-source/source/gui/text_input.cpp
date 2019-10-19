#include <gui>
#include <codecvt>
#include <locale>

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	TextInput::TextInput() {}
	
	void TextInput::create() {
		current_text = "prova";
		dynamic_text = gui::SimpleText("dynamic");
	}

	void TextInput::render() {
		if (CharCodepointPressed != -1){
			wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
			wstring wchar = L" ";
			wchar[0] = CharCodepointPressed;
			string c = converter.to_bytes(wchar);
			current_text += c;
		}
		dynamic_text.render_dynamic(current_text, "tahoma_15px", 100.f, 100.f, vec4(255.f), "left", "normal");

	}

	TextInput::~TextInput() {}
};
