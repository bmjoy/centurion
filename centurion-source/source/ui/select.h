#ifndef SELECT_H
#define SELECT_H

#include <primitives.h>

namespace gui {
	
	class Rectangle;
	class SimpleText;

	class Select {
	public:
		Select(bool options = true);
		void create(float x, float y, float w, float h, vector<string> options, int pickingID);
		void render(bool picking, vec4 color);
		void open_close();
		void close();
		void select_option(int i);
		void select_next();
		void select_previous();
		int get_clicked_option();
		string selectedText;
		~Select();
	private:
		Rectangle* back;
		Rectangle* back_options;
		vector<Rectangle*> back_options_picking;
		SimpleText* text;
		vector<SimpleText*> optionsText;
		float width, height, x, y;
		vector<string> form_options;
		vec2 mainTextPos;
		bool boolOptions;
		bool isOpened;
		bool hasText;
		int nOptions;
		int selectedTextID;
	};
};


#endif