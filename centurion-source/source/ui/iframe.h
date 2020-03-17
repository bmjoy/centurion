#ifndef IFRAME_H
#define IFRAME_H

#include <primitives.h>

namespace gui {
	
	class Image;
	class Button;
	class SimpleText;

	class Iframe {
	public:
		Iframe();
		Iframe(string _name);
		void AddImage(Image* img) { listOfImages.push_back(img); }
		void AddButton(Button* btn) { listOfButtons.push_back(btn); }
		void AddText(SimpleText* txt) { listOfTexts.push_back(txt); }
		void Create(int xPos, int yPos, int width, int height);
		void Render();
		~Iframe();
	private:
		Image* back;
		Image* topleft;
		Image* topright;
		Image* bottomright;
		Image* bottomleft;
		Image* right;
		Image* left;
		Image* top;
		Image* bottom;
		vector<Image*> listOfImages;
		vector<Button*> listOfButtons;
		vector<SimpleText*> listOfTexts;
		string name;
		int x, y, w, h;
	};
};

#endif