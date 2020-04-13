#include <ui.h>
#include <engine.h>
#include <picking.h>
#include <encoding.hpp>

#include <GLFW/glfw3.h>

using namespace std;
using namespace glm;

namespace gui 
{
	TextInput::TextInput(void) 
	{
		placeholder_font_weight = "normal";
		displayRectangle = true;
		this->isActive = false;
	}
	
	void TextInput::Create(const int _id, const int picking_id, const int x, const int y, const int width, const std::string font, const bool hasBackground, glm::vec4 backCol, const glm::vec4 borderCol, const std::string fontWeight, const std::wstring placeholderText) 
	{
		xPos = (float)x; 
		yPos = (float)y; 
		max_chars = TEXT_INPUT_MAX_CHARS_DEFAULT;
		id = _id;
		backColor = backCol;
		borderColor = borderCol;
		displayRectangle = hasBackground;
		placeholder_font = font;
		placeholder_font_weight = fontWeight;

		placeholder_text = gui::SimpleText("static");
		placeholder_text.create_static(placeholderText, placeholder_font, xPos, yPos, "left", "normal", vec4(255.f), placeholder_font_weight);

		float rectHeight = placeholder_text.GetTextHeight();

		text_cursor.create("filled", 0, 0, 2.f, 15.f, "bottom-left", 0);
		cursorPosition = (int)placeholderText.size();

		rectangle = gui::Rectangle();
		rectangle.create("border-filled", xPos, yPos, (float)width, rectHeight, "bottom-left", picking_id);

		for (int i = 0; i < TEXT_INPUT_MAX_CHARS_DEFAULT; i++) currentText[i] = 0;
		for (int i = 0; i < placeholderText.size(); i++) currentText[i] = (int)placeholderText[i];

	}

	void TextInput::RenderTextInput(bool picking) 
	{
		if (picking)
		{
			rectangle.render(vec4(), vec4(), true);
		}
		else
		{
			if (Engine::Mouse::LeftClick)
			{
				if (rectangle.IsClicked(Picking::UI::GetLeftClickId()))
				{
					this->EnableTextInput();
					placeholder_text.create_static(currentText, cursorPosition + 1, placeholder_font, xPos, yPos, "left", "normal", vec4(255.f), placeholder_font_weight);
				}
				else
				{
					this->DisableTextInput();
				}
			}

			if (displayRectangle) rectangle.render(backColor, borderColor, false);

			if (isActive) 
			{
				if (Engine::Keyboard::GetCharCodepointPressed() != -1 && cursorPosition <= max_chars) 
				{
					currentText[cursorPosition] = Engine::Keyboard::GetCharCodepointPressed();
					cursorPosition++;
					placeholder_text.create_static(currentText, cursorPosition + 1, placeholder_font, xPos, yPos, "left", "normal", vec4(255.f), placeholder_font_weight);
				}

				if (Engine::Keyboard::IsKeyNotReleased(GLFW_KEY_BACKSPACE) && cursorPosition > 0) 
				{
					currentText[cursorPosition - 1] = 0;
					cursorPosition--;
					placeholder_text.create_static(currentText, cursorPosition + 1, placeholder_font, xPos, yPos, "left", "normal", vec4(255.f), placeholder_font_weight);
				}

				//if (Engine::Keyboard::IsKeyNotReleased(GLFW_KEY_LEFT) && cursorPosition > 0)
				//{
				//	cursorPosition--;
				//	Engine::Keyboard::SetKeyStatus(GLFW_KEY_LEFT, 0);
				//}

				text_cursor.render(vec4(255.f), vec4(), false, 0, xPos + placeholder_text.get_width(cursorPosition) + 1, yPos + 1);
			}
			placeholder_text.render_static();
		}
		
	}

	void TextInput::EnableTextInput(void)
	{
		this->isActive = true;
	}

	void TextInput::DisableTextInput(void)
	{
		this->isActive = false;
	}

	std::string TextInput::GetText(void)
	{
		std::string text;
		for (int i = 0; i < TEXT_INPUT_MAX_CHARS_DEFAULT; i++) 
		{
			if (currentText[i] != 0)
			{
				text += (char)currentText[i];
			}
		}
		return text;
	}

	void TextInput::Reset(void)
	{
		for (int i = 0; i < TEXT_INPUT_MAX_CHARS_DEFAULT; i++) 
		{
			if (currentText[i] != 0)
			{
				currentText[i] = 0;
			}
		}
		placeholder_text.create_static(currentText, cursorPosition + 1, placeholder_font, xPos, yPos, "left", "normal", vec4(255.f), placeholder_font_weight);
	}

	void TextInput::UpdatePlaceholder(std::string newPlaceholder)
	{
		std::wstring widestring = Encode::GetWideString(newPlaceholder.c_str());
		placeholder_text.SetNewText(widestring);
		for (int i = 0; i < TEXT_INPUT_MAX_CHARS_DEFAULT; i++) 
			currentText[i] = 0;
		for (int i = 0; i < newPlaceholder.size(); i++) 
			currentText[i] = (int)newPlaceholder[i];
		cursorPosition = (int)newPlaceholder.size();
	}

	TextInput::~TextInput(void) {}
};
