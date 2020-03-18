#ifndef LIST_H
#define LIST_H

#include <primitives.h>

#include "simple_text.h"
#include "rectangle.h"

namespace gui {
	
	class TextList {
	public:
		TextList();
		static TextList* GetTextListById(int id);
		string GetSelectedOption() { return selectedOption; }
		void Create(int _id, int _x, int _y, string _font, vec4 _color, vec4 _backColor, int _pickingId);
		void Update(vector<string> _options);
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

		static map<int, TextList*> TextLists;
	};
	
};


#endif