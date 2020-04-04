#ifndef TEXT_BOX_H
#define TEXT_BOX_H

#include <bitmap_font.h>

namespace gui {

	/// <summary>
	/// Class of TextBox
	/// </summary>
	class TextBox {
	public:
		TextBox();

		/// <summary>
		/// This function create a text box
		/// </summary>
		/// <param name="text">text in the textbox</param>
		/// <param name="font">font of the text</param>
		/// <param name="x">x position of the textbox</param>
		/// <param name="y">y position of the textbox</param>
		/// <param name="width">width of the textbox</param>
		/// <param name="height">height of the textbox</param>
		/// <param name="halign">horizontal align of textbox</param>
		/// <param name="valign">vertical align of textbox</param>
		/// <param name="color">color of the textbox</param>
		/// <param name="fontWeight">font weight in textbox</param>
		void create(std::string text, std::string font, float x, float y, float width, float height, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight);

		/// <summary>
		/// this function rendere the textbox
		/// </summary>
		void render();
		~TextBox();
	private:
		std::vector<BitmapFont::StaticTextData> lines_data;
		std::vector<std::string> lines;
		std::string hAlign, vAlign;
	};
};

#endif