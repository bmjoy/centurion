/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "shader.h"

/// <summary>
/// This class is used by UI objects which have to render static or dynamic text.
/// </summary>
class BitmapFont : public Shader
{
public:
	/// <summary>
	/// Public constructor.
	/// </summary>
	BitmapFont();

	/// <summary>
	/// This function sets the text align.
	/// </summary>
	/// <param name="hAlign">The horizontal alignment. Supported values are "left" and "center";</param>
	/// <param name="vAlign">The vertical alignment. Supported values are "normal" and "middle";</param>
	void set_align(string hAlign = "left", string vAlign = "normal");

	/// <summary>
	/// This function initializes and creates the BitmapFont shader with all fonts and settings.
	/// </summary>
	void create();

	/// <summary>
	/// This function provides static data for static text. 
	/// </summary>
	/// <param name="font">The font name;</param>
	/// <param name="text">The text to render;</param>
	/// <param name="x">The X-position;</param>
	/// <param name="y">The Y-position;</param>
	/// <param name="bold">The font-weight. True = "bold", false = "normal";</param>
	/// <param name="line_number">The line of the current word/text. This is used in the case of text-boxes;</param>
	/// <returns>The StaticData used to render the text with "render_static" function.</returns>
	txt::StaticData create_static(string &font, string &text, float x, float y, bool bold = false, int line_number = 0);

	/// <summary>
	/// This function is used to dynamically render a text. It's useful when the text is changing
	/// every frame or very often. 
	/// </summary>
	/// <param name="font">The font name;</param>
	/// <param name="xPos">The X-position;</param>
	/// <param name="yPos">The Y-position;</param>
	/// <param name="text">The text to render;</param>
	/// <param name="color">The text color;</param>
	/// <param name="shadow"></param>
	/// <param name="bold"></param>
	void render_dynamic(string &font, float xPos, float yPos, string &text, vec4 &color, bool shadow, bool bold = false);

	/// <summary>
	/// This function is used to render a text created with "create_static" function. 
	/// </summary>
	/// <param name="data">The StaticData information, usually provided by "create_static" function.</param>
	void render_static(txt::StaticData &data);
	
	/// <summary>
	/// The destructor.
	/// </summary>
	~BitmapFont();

private:
	/// <summary>
	/// This function checks if the codepoint character is arabic or not.
	/// </summary>
	/// <param name="codepoint">The codepoint character, usually an integer or unsigned integer.</param>
	/// <returns>True if arabic, false if not arabic.</returns>
	bool isArabic(int codepoint);

	map<int, txt::Character> fontData[10];
	map<string, int> fontIdMap;
	string h_align, v_align, path;
	int total_width;
	int offset_x;
	map<string, int> hAlignMap;
	map<string, int> vAlignMap;

};

static BitmapFont _txt;

#endif
