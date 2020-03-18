#include <ui.h>
#include <text_list.h>

#include <engine.h>
#include <picking.h>

#include <translationsTable.h>

namespace gui {

	map<int, TextList*> TextList::TextLists;

	TextList::TextList() {
		optionsHeight = 25.f;
		selectedOption = "";
	}

	TextList * TextList::GetTextListById(int id)
	{
		if (TextLists.count(id) > 0) {
			return TextLists[id];
		}
		else {
			return nullptr;
		}
	}

	void TextList::Create(int _id, int _x, int _y, string _font, vec4 _color, vec4 _backColor, int _pickingId) {
		id = _id;
		x = _x;
		y = _y;
		font = _font;
		color = _color;
		pickingId = _pickingId;
		backColor = _backColor;

		TextLists[id] = this;
	}

	void TextList::Update(vector<string> _options)
	{
		options = _options;
		n_options = (int)options.size();
		selectedOption = options[0];
		for (int i = 0; i < options.size(); i++) {
			string option = options[i];
			
			float _y = y - (i + 1) * optionsHeight;

			SimpleText optiontext = SimpleText("static");
			optiontext.create_static(option, font, (float)x, _y, "left", "normal", color, "normal");

			float _w = optiontext.get_width() + 10.f;

			Rectangle optionback = Rectangle();
			optionback.create("filled", (float)x, _y, _w, optionsHeight, "bottom-left", pickingId);

			optionsBack.push_back(optionback);
			optionsText.push_back(optiontext);
		}
	}

	void TextList::Render(bool picking)
	{
		
		if (Engine::Mouse::LeftClick) {
			selectedOption = options[GetIdFromClick()];
			Engine::Mouse::LeftClick = false;
		}
		for (int i = 0; i < n_options; i++) {
			if (picking) {
				optionsBack[i].render(vec4(), true);
			}
			else {
				if (options[i] == selectedOption) {
					optionsBack[i].render(backColor, false);
				}
				optionsText[i].render_static();
			}
		}
	}

	int TextList::GetIdFromClick()
	{
		int yMouse = (int)Engine::Mouse::GetYPosition() / (int)optionsHeight;
		int yRel = y / (int)optionsHeight - 1;
		int pos = yRel - yMouse;
		if (pos < 0 || pos >= n_options) return 0;
		return pos;
	}

	TextList::~TextList() {}
};
