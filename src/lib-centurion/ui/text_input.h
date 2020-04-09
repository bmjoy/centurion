/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#ifndef TEXT_INPUT_MAX_CHARS_DEFAULT
#define TEXT_INPUT_MAX_CHARS_DEFAULT 200
#endif 

#include <gl_items.h>

namespace gui {

	/// <summary>
	/// Class of TextInput
	/// </summary>
	class TextInput {
	public:
		TextInput();

		/// <summary>
		/// This function creates the text input
		/// </summary>
		/// <param name="_id">the text input id</param>
		/// <param name="picking_id">the text input picking id</param>
		/// <param name="x">x position of text input</param>
		/// <param name="y">y position of the text input</param>
		/// <param name="width">The width of the textinput</param>
		/// <param name="font">The font name</param>
		/// <param name="fontWeight">The font weight (normal - bold)</param>
		/// <param name="backCol">The background color</param>
		/// <param name="borderCol">The border color</param>
		/// <param name="maxChars">max characters of text input</param>
		/// <param name="text">the placeholder text as wide string</param>
		void Create(const int _id, const int picking_id, const int x, const int y, const int width, const std::string font, const bool hasBackground, const glm::vec4 backCol, const glm::vec4 borderCol, const std::string fontWeight = "normal", const std::wstring placeholderText = L"");

		/// <summary>
		/// This function renders the Input text
		/// </summary>
		/// <param name="picking">Boolean: true = picking is active, false = normal rendering</param>
		void Render(bool picking = false);

		/// <summary>
		/// This function enables the text input
		/// </summary>
		void Enable(void) { isActive = true; }

		/// <summary>
		/// This function disables the text input
		/// </summary>
		void Disable(void) { isActive = false; }

		/// <summary>
		/// This method resets the whole TextInput
		/// </summary>
		void Reset(void);

		/// <summary>
		/// This method returns the text
		/// </summary>
		/// <returns></returns>
		std::string GetText(void);

		/// <summary>
		/// This method updates the text input placeholder with a new one
		/// </summary>
		/// <param name="newPlaceholder">A std::string indicating the new placeholder text</param>
		void UpdatePlaceholder(std::string newPlaceholder);

		~TextInput();
	private:
		gui::Rectangle rectangle;
		glm::vec4 backColor, borderColor;
		bool displayRectangle;

		gui::Rectangle text_cursor;
		gui::SimpleText placeholder_text;
		std::string placeholder_font;
		std::string placeholder_font_weight;
		bool isActive;
		float xPos, yPos;
		int cursorPosition;
		int max_chars;
		int currentText[TEXT_INPUT_MAX_CHARS_DEFAULT];
		unsigned int id;
	};
};

#endif