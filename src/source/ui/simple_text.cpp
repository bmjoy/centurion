#include <ui.h>

#include <engine.h>

#include <logger.h>
#include "bitmap_font.h"

using namespace std;
using namespace glm;

namespace gui {

	float SimpleText::CalculateTextWidth(string text, string font, string fontWeight)
	{
		gui::SimpleText temptext = gui::SimpleText("static");
		temptext.create_static(text, font, 0, 0, "center", "middle", vec4(255), fontWeight);
		return temptext.get_width();
	}

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
		staticData = Text()->create_static(fontName, text.c_str(), x, y, isBold);
		staticData.y = y;
		staticData.color = color;
		staticData.shadow = shadow;
	}

	void SimpleText::create_static(const int * codepoints, int textSize, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color, std::string fontWeight)
	{
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
		staticData = Text()->create_static(fontName, codepoints, textSize, x, y, isBold);
		staticData.y = y;
		staticData.color = color;
		staticData.shadow = shadow;
	}

	void SimpleText::render_static() {
		if (type == "dynamic") Logger::Info("Dynamic text rendered with static option");
		Text()->set_align(hAlign, vAlign);		
		Text()->render_static(staticData);
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
			tot_width = (float)staticData.totalWidth;
		}
		else {
			int j = std::min(pos, staticData.textSize);
			for (int i = 0; i < j; ++i) {
				tot_width += staticData.charsWidth[i];
			}
		}
		return tot_width;
	}

	SimpleText::~SimpleText(){}
};