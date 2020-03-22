#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <bitmap_font.h>

namespace gui {
	class TextBox {
	public:
		TextBox();
		void create(string text, string font, float x, float y, float width, float height, string halign, string valign, vec4 color, string fontWeight);
		void render();
		~TextBox();
	private:
		vector<BitmapFont::StaticTextData> lines_data;
		vector<string> lines;
		string hAlign, vAlign;
	};
};

#endif