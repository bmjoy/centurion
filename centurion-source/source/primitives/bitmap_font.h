#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "shader.h"

namespace txt {
	bool isArabic(int codepoint);
};

class BitmapFont : public Shader
{
public:
	BitmapFont();
	void set_align(string hAlign = "left", string vAlign = "normal");
	void create();
	void render_dynamic(string &font, float xPos, float yPos, string &text, vec4 &color, bool shadow, bool bold = false);
	txt::StaticData create_static(string &font, string &text, float x, float y, bool bold = false, int line_number = 0);
	void render_static(txt::StaticData &data);
	int getShaderId() { return shaderId; }
	~BitmapFont();

private:
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