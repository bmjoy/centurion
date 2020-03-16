#ifndef SIMPLE_TEXT_H
#define SIMPLE_TEXT_H

#include <primitives.h>

namespace gui {
	class SimpleText {
	public:
		SimpleText(string Type = "static", bool Shadow = false);
		void create_static(string text, string font, float x, float y, string halign, string valign, vec4 color, string fontWeight = "normal");
		void render_static();
		void render_dynamic(string text, string Font, float x, float y, vec4 color, string halign, string valign, string fontWeight = "normal");
		float get_width(int pos = 0);
		~SimpleText();
	private:
		string type;
		txt::StaticData data;
		string hAlign, vAlign;
		bool shadow;
	};
};

#endif