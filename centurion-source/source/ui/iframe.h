#ifndef IFRAME_H
#define IFRAME_H

#include <primitives.h>

#include "button.h"
#include "text_list.h"

namespace gui {
	
	class Iframe {
	public:
		Iframe();
		Iframe(string _name);
		void AddImage(Image img) { listOfImages.push_back(img); }
		void AddButton(Button btn) { listOfButtons.push_back(btn); }
		void AddText(SimpleText txt) { listOfTexts.push_back(txt); }
		void Clear();
		void AddTextList(TextList* txtList);
		void Create(int xPos, int yPos, int width, int height);
		void Render(bool picking = false);
		~Iframe();
	private:
		void RenderImages(bool picking);
		void RenderButtons(bool picking);
		void RenderTexts();
		void RenderTextLists(bool picking);
		vector<Image> listOfImages;
		vector<Button> listOfButtons;
		vector<SimpleText> listOfTexts;
		vector<TextList*> listOfTextLists;
		Image back;
		Image topleft;
		Image topright;
		Image bottomright;
		Image bottomleft;
		Image right;
		Image left;
		Image top;
		Image bottom;
		string name;
		int x, y, w, h;
	};
};

#endif