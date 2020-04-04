#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <primitives.h>

namespace gui {

	/// <summary>
	/// Class of TextInput
	/// </summary>
	class TextInput {
	public:
		TextInput();

		/// <summary>
		/// This function create the text input
		/// </summary>
		/// <param name="text">take the text as string</param>
		/// <param name="x">x position of text input</param>
		/// <param name="y">y position of the text input</param>
		/// <param name="maxChars">max characters of text input</param>
		void create(std::string text, float x, float y, int maxChars);

		/// <summary>
		/// This function render the Input text
		/// </summary>
		void render();

		/// <summary>
		/// This function active the text input
		/// </summary>
		/// <param name="b">??? boolean type; default is true</param>
		void active(bool b = true) { is_active = b; }
		std::string get_text();
		~TextInput();
	private:
		gui::Rectangle text_cursor;
		gui::SimpleText static_text;
		bool is_active;
		float xPos, yPos;
		int cursorPosition;
		int max_chars;
		int currentText[100];
	};
};

#endif