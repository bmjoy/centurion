#ifndef LIST_H
#define LIST_H

#define MAX_NUMBER_OF_TEXT_LISTS 100

#include <primitives.h>

#include "simple_text.h"
#include "rectangle.h"

namespace gui {
	class TextList {
	public:
		TextList();
		static void AddTextListToArray(int id, TextList* txtList);
		static TextList* GetTextListById(int id);
		static void UpdateTextListById(int id, vector<string> *_options);
		int GetId() { return id; }
		string GetSelectedOption() { return selectedOption; }
		void Create(int _id, int _x, int _y, string _font, vec4 _color, vec4 _backColor, int _pickingId);
		void Update(vector<string> *_options);
		void Render(bool picking);
		~TextList();

	private:
		int GetIdFromClick();
		vector<Rectangle> optionsBack;
		vector<SimpleText> optionsText;
		vector<string> options;
		string font;
		string selectedOption;
		vec4 color, backColor;
		float optionsHeight;
		int x, y;
		int pickingId;
		int n_options;
		int id;

		static array<TextList*, MAX_NUMBER_OF_TEXT_LISTS> TextLists;
	};
};



#endif