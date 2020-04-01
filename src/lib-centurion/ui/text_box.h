#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <bitmap_font.h>

namespace gui {
	class TextBox {
	public:
		TextBox();
		void create(std::string text, std::string font, float x, float y, float width, float height, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight);
		void render();
		~TextBox();
	private:
		std::vector<BitmapFont::StaticTextData> lines_data;
		std::vector<std::string> lines;
		std::string hAlign, vAlign;
	};
};

#endif