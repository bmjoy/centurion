#include <gui>

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
		if (type == "dynamic") cout << "DEBUG: Dynamic text created with static option\n";
		if (x < 0) x += getParam("window-width");
		if (y < 0) y += getParam("window-height");

		hAlign = halign;
		vAlign = valign;

		string fontName = font;
		bool isBold = false;
		if (fontWeight == "bold") {
			fontName = fontName + "_bold";
			isBold = true;
		}
		data = obj::Text()->create_static(fontName, text, x, isBold);
		data.y = y;
		data.color = color;
		data.shadow = shadow;
	}

	void SimpleText::render_static() {
		if (type == "dynamic") cout << "DEBUG: Dynamic text rendered with static option\n";
		obj::Text()->set_align(hAlign, vAlign);		
		obj::Text()->render_static(data);
	}
	void SimpleText::render_dynamic(string text, string Font, float x, float y, vec4 color, string halign, string valign, string fontWeight) {
		if (type == "static") cout << "DEBUG: Static text rendered with dynamic option\n";
		obj::Text()->set_align(halign, valign);
		if (fontWeight == "normal") {
			obj::Text()->render_dynamic(Font, x, y, text, color, shadow, false);
		}
		else {
			obj::Text()->render_dynamic(Font, x, y, text, color, shadow, true);
		}
	}

	SimpleText::~SimpleText(){}
};