#include <gui>
#include <global>
#include <engine.h>

#include <logger.h>
#include "bitmap_font.h"

namespace gui {

	using namespace std;
	using namespace glm;
	using namespace glb;

	SimpleText::SimpleText(string Type, bool Shadow) {
		type = Type;
		hAlign = "left";
		vAlign = "normal";
		shadow = Shadow;
	}

	void SimpleText::create_static(string text, string font, float x, float y, string halign, string valign, vec4 color, string fontWeight) {
		if (type == "dynamic") Logger::Info("Dynamic text created with static option");
		if (x < 0) x += Engine::myWindow::Width;
		if (y < 0) y += Engine::myWindow::Height;

		hAlign = halign;
		vAlign = valign;

		string fontName = font;
		bool isBold = false;
		if (fontWeight == "bold") {
			fontName = fontName + "_bold";
			isBold = true;
		}
		data = Text()->create_static(fontName, text, x, y, isBold);
		data.y = y;
		data.color = color;
		data.shadow = shadow;
	}

	void SimpleText::render_static() {
		if (type == "dynamic") Logger::Info("Dynamic text rendered with static option");
		Text()->set_align(hAlign, vAlign);		
		Text()->render_static(data);
	}
	void SimpleText::render_dynamic(string text, string Font, float x, float y, vec4 color, string halign, string valign, string fontWeight) {
		if (type == "static") Logger::Info("Static text rendered with dynamic option");
		Text()->set_align(halign, valign);
		if (fontWeight == "normal") {
			Text()->render_dynamic(Font, x, y, text, color, shadow, false);
		}
		else {
			Text()->render_dynamic(Font, x, y, text, color, shadow, true);
		}
	}

	float SimpleText::get_width(int pos) {
		float tot_width = 0.f;
		if (pos == 0) {
			tot_width = (float)data.totalWidth;
		}
		else {
			int j = std::min(pos, data.textSize);
			for (int i = 0; i < j; ++i) {
				tot_width += data.charsWidth[i];
			}
		}
		return tot_width;
	}

	SimpleText::~SimpleText(){}
};