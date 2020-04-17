#include <ui.h>
#include <text_list.h>

#include <engine.h>
#include <picking.h>
#include <hector-lua.h>

#include <encoding.hpp>

#include <translationsTable.h>

using namespace std;
using namespace glm;

namespace gui {

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
		isHidden = false;
		isClicked = false;
	}

	void TextList::Create(int _x, int _y, string _font, vec4 _color, vec4 _backColor, int _pickingId, const std::string& luaCmd, const unsigned int _maxOptions, const unsigned int _borderWidth) {
		x = _x;
		y = _y;
		font = _font;
		color = _color;
		pickingId = _pickingId;
		backColor = _backColor;
		luaCommand = luaCmd;
		maxOptions = _maxOptions;
		(_borderWidth == BORDERWIDTH_DEFAULT) ? borderWidth = 50.f : borderWidth = _borderWidth;
		isHidden = false;
		isClicked = false;
	}

	void TextList::Update(vector<string>* _options, const std::string prefix)
	{
		optionsBack.clear();
		optionsText.clear();

		options = vector<string>(*_options);
		nOptions = (int)options.size();
		firstOption = 0;
		lastOption = std::min(maxOptions, nOptions) - 1;

		border = gui::Rectangle();

		float borderheight = optionsHeight * maxOptions;
		selectedOption = options[0];
		for (int i = 0; i < options.size(); i++) {
			wstring option;
			if (prefix == "")
			{
				option = Encode::GetWideString(options[i].c_str());
			}
			else {
				option = TranslationsTable::GetWTranslation(prefix + options[i]);
			}

			float _y = y - (i + 1) * optionsHeight;

			SimpleText optiontext = SimpleText("static");
			optiontext.create_static(option, font, (float)x, _y, "left", "normal", color, "normal");

			Rectangle optionback = Rectangle();
			optionback.create("filled", (float)x - 2.f, _y, borderWidth, optionsHeight, "bottom-left", pickingId);

			optionsBack.push_back(optionback);
			optionsText.push_back(optiontext);
		}

		border.create("border", x - 2.f, (float)y, (float)borderWidth, borderheight, "top-left", 0);

		minX = x;
		maxX = x + (int)borderWidth;
		minY = y - (int)borderheight;
		maxY = y;
	}

	void TextList::Render(bool picking)
	{
		if (isHidden) return;

		deltaY = 0.f;
		if (MouseIsHover() && !picking) {
			ScrollOptions();
		}

		IsClicked();

		for (int i = 0; i < nOptions; i++) {

			optionsBack[i].IncreaseYPosition(deltaY);
			optionsText[i].IncreaseYPosition(deltaY);

			if (picking) {
				if (i >= firstOption && i <= lastOption) {
					optionsBack[i].render(vec4(), vec4(), true);
				}
			}

			else {

				if (i >= firstOption && i <= lastOption) {
					optionsBack[i].render(backColor, vec4(), false);
				}

				if (i >= firstOption && i <= lastOption) {
					optionsText[i].render_static();
				}
			}
		}

		border.render(vec4(), borderColor);
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

	bool TextList::IsClicked(void)
	{
		if (Engine::Mouse::LeftClick && Picking::UI::GetLeftClickId() == pickingId)
		{
			selectedOption = options[GetIdFromClick()];
			Hector::ExecuteCommand(luaCommand);
			return true;
		}
		else {
			return false;
		}
	}

	TextList::~TextList() {}
};