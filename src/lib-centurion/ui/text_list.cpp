#include <ui.h>
#include <text_list.h>

#include <engine.h>
#include <picking.h>
#include <hector-lua.h>

#include <translationsTable.h>

using namespace std;
using namespace glm;

namespace gui {
	array<TextList*, MAX_NUMBER_OF_TEXT_LISTS> TextList::TextLists = { nullptr };

	TextList::TextList() {
		optionsHeight = 25.f;
		selectedOption = "";
		options = vector<string>();
		borderColor = glm::vec4(255.f);
		firstOption = 0;
		lastOption = 0;
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
		deltaY = 0;
	}

	void TextList::AddTextListToArray(int id, TextList * txtList)
	{
		if (id < 0 || id > MAX_NUMBER_OF_TEXT_LISTS) return;
		TextLists[id] = txtList;
	}

	TextList * TextList::GetTextListById(int id)
	{
		if (id < 0 || id > MAX_NUMBER_OF_TEXT_LISTS) return nullptr;

		return TextLists[id];

	}

	void TextList::UpdateTextListById(int id, vector<string>* _options, const std::string prefix)
	{
		TextLists[id]->Update(_options, prefix);
	}

	void TextList::Create(int _id, int _x, int _y, string _font, vec4 _color, vec4 _backColor, int _pickingId, const std::string & luaCmd, const unsigned int _maxOptions, const unsigned int _borderWidth) {
		id = _id;
		x = _x;
		y = _y;
		font = _font;
		color = _color;
		pickingId = _pickingId;
		backColor = _backColor;
		luaCommand = luaCmd;
		maxOptions = _maxOptions;
		borderWidth = _borderWidth;
		AddTextListToArray(this->id, this);
	}

	void TextList::Update(vector<string> *_options, const std::string prefix)
	{
		optionsBack.clear();
		optionsText.clear();

		options = vector<string>(*_options);
		nOptions = (int)options.size();
		firstOption = 0;
		lastOption = std::min(maxOptions, nOptions) - 1;

		border = gui::Rectangle();

		float borderheight = optionsHeight * maxOptions;
		float borderwidthvar = 0.f;
		selectedOption = options[0];
		for (int i = 0; i < options.size(); i++) {
			wstring option = TranslationsTable::GetWTranslation(prefix + options[i]);

			float _y = y - (i + 1) * optionsHeight;

			SimpleText optiontext = SimpleText("static");
			optiontext.create_static(option, font, (float)x, _y, "left", "normal", color, "normal");

			float _w = optiontext.get_width() + 10.f;

			Rectangle optionback = Rectangle();
			optionback.create("filled", (float)x - 2.f, _y, _w + 4.f, optionsHeight, "bottom-left", pickingId);

			optionsBack.push_back(optionback);
			optionsText.push_back(optiontext);

			borderwidthvar = std::max(borderwidthvar, _w);
		}

		if (borderWidth == BORDERWIDTH_DEFAULT)
		{
			borderWidth = borderwidthvar;
		}

		border.create("border", x - 2.f, y, borderWidth + 4.f, borderheight, "top-left", 0);

		minX = x;
		maxX = x + (int)borderWidth;
		minY = y - (int)borderheight;
		maxY = y;
	}

	void TextList::Render(bool picking)
	{
		deltaY = 0.f;
		if (MouseIsHover() && picking) {
			ScrollOptions();
		}

		int x = GetIdFromClick();

		if (Engine::Mouse::LeftClick && Picking::leftClickID_UI == pickingId) {
			selectedOption = options[GetIdFromClick()];
			Engine::Mouse::LeftClick = false;

			Hector::ExecuteCommand(luaCommand);
		}

		for (int i = 0; i < nOptions; i++) {

			optionsBack[i].IncreaseYPosition(deltaY);
			optionsText[i].IncreaseYPosition(deltaY);

			if (picking) {
				if (i >= firstOption && i <= lastOption) {
					optionsBack[i].render(vec4(), true);
				}
			}

			else {
				if (options[i] == selectedOption) {
					if (i >= firstOption && i <= lastOption) {
						optionsBack[i].render(backColor, false);
					}
				}
				if (i >= firstOption && i <= lastOption) {
					optionsText[i].render_static();
				}
			}
		}

		border.render(borderColor);
	}

	int TextList::GetIdFromClick()
	{
		int yMouse = (-1) * ((int)Engine::Mouse::GetYPosition() - y) / (int)optionsHeight;
		yMouse += firstOption;
		if (yMouse < 0) yMouse = firstOption;
		if (yMouse > lastOption) yMouse = lastOption;
		return yMouse;
	}

	void TextList::ScrollOptions(void)
	{
		if (Engine::Mouse::ScrollBool)
		{
			int scrollvalue = 2 * int(Engine::Mouse::ScrollValue < 0) - 1;
			if ((scrollvalue > 0 && lastOption < nOptions - 1) || (scrollvalue < 0 && firstOption > 0)) {
				firstOption += scrollvalue;
				lastOption += scrollvalue;
				deltaY = scrollvalue * optionsHeight;
			}
		}
	}

	bool TextList::MouseIsHover(void)
	{
		if (Engine::Mouse::GetXPosition() > maxX || Engine::Mouse::GetXPosition() < minX) return false;
		if (Engine::Mouse::GetYPosition() > maxY || Engine::Mouse::GetYPosition() < minY) return false;
		return true;
	}

	TextList::~TextList() {}
};