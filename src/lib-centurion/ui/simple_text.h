/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <bitmap_font.h>

namespace gui {

	/// <summary>
	/// Class of SimpleText
	/// </summary>
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

		/// <summary>
		/// This function returns the font height
		/// </summary>
		float GetTextHeight(void) { return (float)staticData.fontHeight; }

		/// <summary>
		/// This function create a static text.
		/// </summary>
		/// <param name="text"></param>
		/// <param name="font"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="halign"></param>
		/// <param name="valign"></param>
		/// <param name="color"></param>
		/// <param name="fontWeight"></param>
		void create_static(std::string text, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");
		void create_static(std::wstring wtext, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");
		void create_static(const int* codepoints, int textSize, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight = "normal");

		/// <summary>
		/// This function rendere static text.
		/// </summary>
		void render_static();

		/// <summary>
		/// This function rendere a dinamyc text.
		/// </summary>
		/// <param name="text">text content</param>
		/// <param name="Font">Font of the text</param>
		/// <param name="x">X position of the text</param>
		/// <param name="y">Y position of the text</param>
		/// <param name="color">Color of the text</param>
		/// <param name="halign">height align of the text</param>
		/// <param name="valign">vertical align of the text</param>
		/// <param name="fontWeight">font weight</param>
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