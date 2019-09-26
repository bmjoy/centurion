#pragma once

#include <global.hpp>
#include <objects.h>

#include "ui_object.h"

namespace gui {
	class SimpleText : public UIObject
	{
	public:
		SimpleText(std::string Type = "static", bool Shadow = false);
		void create_static(std::string text, std::string font, float x, float y, std::string halign, std::string valign, glm::vec4 color);
		void render_static();
		void render_dynamic(std::string Font, glm::vec4 color, std::string halign, std::string valign);
		float get_width() { return (float)data.totalWidth; }
		~SimpleText();
	private:
		std::string type;
		txt::StaticData data;
		std::string hAlign, vAlign;
		bool shadow;
	};
}
