#ifndef SELECT_H
#define SELECT_H

#include <primitives.h>

#include "rectangle.h"
#include "simple_text.h"

namespace gui {
	
	class Select {
	public:
		Select(bool options = true);
		void create(float x, float y, float w, float h, std::vector<std::string> options, int pickingID);
		void render(bool picking, glm::vec4 color);
		void open_close();
		void close();
		void select_option(int i);
		void select_next();
		void select_previous();
		int get_clicked_option();
		std::string selectedText;
		~Select();
	private:
		Rectangle back;
		Rectangle back_options;
		std::vector<Rectangle> back_options_picking;
		SimpleText text;
		std::vector<SimpleText> optionsText;
		float width, height, x, y;
		std::vector<std::string> form_options;
		glm::vec2 mainTextPos;
		bool boolOptions;
		bool isOpened;
		bool hasText;
		int nOptions;
		int selectedTextID;
	};
};


#endif