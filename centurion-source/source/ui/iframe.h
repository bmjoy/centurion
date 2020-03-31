/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef IFRAME_H
#define IFRAME_H

#include <primitives.h>

#include "button.h"
#include "text_list.h"

namespace gui {
	
	/// <summary>
	/// This class handles Iframes, dynamic windows that list one or more files.
	/// </summary>
	class Iframe {
	public:
		Iframe();
		Iframe(std::string _name);

		/// <summary>
		/// This function adds an image to the iframe window.
		/// </summary>
		/// <param name="img">Image that would be added. It only supports Image classes.</param>
		void AddImage(Image img) { listOfImages.push_back(img); }

		/// <summary>
		/// This function adds a button to the iframe window.
		/// </summary>
		/// <param name="btn">Button that would be added. It only supports Button classes.</param>
		void AddButton(Button btn) { listOfButtons.push_back(btn); }

		/// <summary>
		/// This function adds a text to the iframe texts list.
		/// </summary>
		/// <param name="txt">Text that would be added. It only supports SimpleText classes.</param>
		void AddText(SimpleText txt) { listOfTexts.push_back(txt); }

		/// <summary>
		/// This function clears the entire texts list. 
		/// </summary>
		void Clear();

		/// <summary>
		/// This function adds a specific texts list to the iframe.
		/// </summary>
		/// <param name="txtList">Texts list that would be added. It only supports referenced TextList classes.</param>
		void AddTextList(TextList* txtList);

		/// <summary>
		/// This function creates an instance of Iframe.
		/// </summary>
		/// <param name="xPos">X postition of the Iframe. It only supports integers.</param>
		/// <param name="yPos">Y postition of the Iframe. It only supports integers.</param>
		/// <param name="width">Width of the Iframe. It only supports integers.</param>
		/// <param name="height">Height of the Iframe. It only supports integers.</param>
		void Create(int xPos, int yPos, int width, int height);

		/// <summary>
		/// This function starts the rendering process of the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void Render(bool picking = false);

		~Iframe();
	private:
		/// <summary>
		/// This function starts the rendering process of images within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderImages(bool picking);
		
		/// <summary>
		/// This function starts the rendering process of buttons within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderButtons(bool picking);

		/// <summary>
		/// This function starts the rendering process of texts within the Iframe text list.
		/// </summary>
		void RenderTexts();

		/// <summary>
		/// This function starts the rendering process of text lists within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderTextLists(bool picking);

		std::vector<Image> listOfImages;
		std::vector<Button> listOfButtons;
		std::vector<SimpleText> listOfTexts;
		std::vector<TextList*> listOfTextLists;
		Image back;
		Image topleft;
		Image topright;
		Image bottomright;
		Image bottomleft;
		Image right;
		Image left;
		Image top;
		Image bottom;
		std::string name;
		int x, y, w, h;
	};
};

#endif