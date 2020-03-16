#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#include <primitives.h>

namespace gui {
	class TextInput {
	public:
		TextInput();
		void create(string text, float x, float y, int maxChars);
		void render();
		void active(bool b = true) { is_active = b; }
		string get_text() { return current_text; }
		~TextInput();
	private:
		gui::Rectangle text_cursor;
		gui::SimpleText static_text;
		string current_text;
		bool is_active;
		float xPos, yPos;
		int cursorPosition;
		int max_chars;
	};
};

#endif