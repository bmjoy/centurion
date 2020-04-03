#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <bitmap_font.h>

namespace gui {
	class SimpleText {
	public:
		static float CalculateTextWidth(std::string text, std::string font, std::string fontWeight = "normal");
		SimpleText(std::string Type = "static", bool Shadow = false);
		
		/// <summary>
		/// This function increases the y position of the text
		/// </summary>
		void IncreaseYPosition(float y) { staticData.y += y; }

		/// <summary>
		/// This function returns the y position of the text
		/// </summary>
		float GetYPosition(void) { return staticData.y; }

		void create_static(std::string text, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");
		void create_static(std::wstring wtext, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");
		void create_static(const int* codepoints, int textSize, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");
		void render_static();
		void render_dynamic(std::string text, std::string Font, float x, float y, glm::vec4 color, std::string halign, std::string valign, std::string fontWeight = "normal");
		float get_width(int pos = 0);
		~SimpleText();
	private:
		std::string type;
		BitmapFont::StaticTextData staticData;
		std::string hAlign, vAlign;
		bool shadow;
	};
};

#endif