#include <ui.h>

#include <engine.h>
#include <picking.h>
#include <encoding.hpp>

#include <translationsTable.h>

using namespace std;
using namespace glm;

namespace gui {

	FormOptions::FormOptions() {
	}

	void FormOptions::Create(const unsigned int pickingId, const int xPos, const int yPos, const int width, const int height, const std::string font, const glm::vec4 _textColor, const glm::vec4 _backColor) {

		w = float(width); h = float(height); x = float(xPos); y = float(yPos);
		fontName = font;
		backColor = _backColor;
		textColor = _textColor;

		selectedOptionText = gui::SimpleText("static");
		selectedOptionText.create_static(L"", fontName, x, y - 12, "left", "middle", textColor);

		selectedOptionBack = gui::Rectangle();
		selectedOptionBack.create("border-filled", x - 2, y, w, h, "top-left", pickingId);

		options = gui::TextList();
		options.Create(x, y - h, fontName, textColor, backColor, Picking::UI::ObtainPickingID(), "", 3, w - 3);
		options.Hide();
	}

	void FormOptions::Render(bool picking) {

		if (picking)
		{
			selectedOptionBack.render(glm::vec4(), glm::vec4(), true);
			options.Render(true);
		}
		else
		{
			if (options.IsClicked() && options.IsHidden() == false) {
				selectedText = Encode::GetWideString(options.GetSelectedOption().c_str());
				selectedOptionText.SetNewText(selectedText);
				options.Hide();
			}

			if (selectedOptionBack.IsClicked(Picking::UI::GetLeftClickId()))
			{
				if (options.IsHidden())
				{
					options.Show();
					options.Update(&this->optionsList, this->prefix);
				}
				else {
					options.Hide();
				}
			}

			selectedOptionBack.render(backColor, glm::vec4(255), false);
			selectedOptionText.render_static();
			options.Render(false);

		}
	}

	void FormOptions::UpdateOptions(std::vector<std::string>* optionsList, std::string prefix)
	{
		this->optionsList = (*optionsList);
		this->prefix = prefix;
		options.Update(&this->optionsList, this->prefix);
		selectedText = Encode::GetWideString(this->optionsList[0].c_str());
		selectedOptionText.SetNewText(selectedText);
	}

	std::wstring FormOptions::GetSelectedOption(void)
	{
		return selectedText;
	}

	FormOptions::~FormOptions() {
	}
};
