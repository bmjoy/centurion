#ifndef IFRAME_H
#define IFRAME_H

#include <primitives.h>

namespace gui {
	
	class Image;
	class Button;

	class Iframe {
	public:
		Iframe();
		Iframe(string _name);
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
		vector<Image*> listOfButtons;
		string name;
		int x, y, w, h;
	};
};

#endif