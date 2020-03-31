#ifndef IFRAME_H
#define IFRAME_H

#include <primitives.h>

#include "button.h"
#include "text_list.h"

namespace gui {
	
	/// <summary>
	/// Class of Iframe;
	/// Iframe are dinamic windows that show scenarios in editor
	/// </summary>
	class Iframe {
	public:

		/// <summary>
		/// Costructor of iFrame
		/// </summary>
		Iframe();

		/// <summary>
		/// This function return a string with name of map
		/// </summary>
		/// <param name="_name">Name of the map</param>
		Iframe(std::string _name);

		/// <summary>
		/// This function insert add image to the list
		/// </summary>
		/// <param name="img">image for the scenario</param>
		void AddImage(Image img) { listOfImages.push_back(img); }

		/// <summary>
		/// This funciotn insert add scenario to the list
		/// </summary>
		/// <param name="btn">add buttun</param>
		void AddButton(Button btn) { listOfButtons.push_back(btn); }

		/// <summary>
		/// This function insert add text to the list
		/// </summary>
		/// <param name="txt">text to show</param>
		void AddText(SimpleText txt) { listOfTexts.push_back(txt); }

		/// <summary>
		/// ??? Clean the terminal 
		/// </summary>
		void Clear();

		/// <summary>
		/// This function add text list
		/// </summary>
		/// <param name="txtList">text of list</param>
		void AddTextList(TextList* txtList);

		/// <summary>
		/// This function create the list
		/// </summary>
		/// <param name="xPos">x postition of the list</param>
		/// <param name="yPos">y position of the list</param>
		/// <param name="width">width dimension of the list</param>
		/// <param name="height">height dimension of the list</param>
		void Create(int xPos, int yPos, int width, int height);

		/// <summary>
		/// Thi function render the list
		/// </summary>
		/// <param name="picking">Default 0</param>
		void Render(bool picking = false);

		/// <summary>
		/// Destructor of Ifram
		/// </summary>
		~Iframe();
	private:

		/// <summary>
		/// Images Render
		/// </summary>
		/// <param name="picking">Default value 0</param>
		void RenderImages(bool picking);
		
		/// <summary>
		/// Buttons render
		/// </summary>
		/// <param name="picking">Defaults value <0/param>
		void RenderButtons(bool picking);

		/// <summary>
		/// Text render
		/// </summary>
		void RenderTexts();

		/// <summary>
		/// Render text list
		/// </summary>
		/// <param name="picking">Defalut value </param>
		void RenderTextLists(bool picking);

		/// <summary>
		/// Return a list of image
		/// </summary>
		std::vector<Image> listOfImages;

		/// <summary>
		/// return a list of buttons
		/// </summary>
		std::vector<Button> listOfButtons;

		/// <summary>
		/// return a list of text
		/// </summary>
		std::vector<SimpleText> listOfTexts;

		/// <summary>
		/// return the text list
		/// </summary>
		std::vector<TextList*> listOfTextLists;

		/// <summary>
		/// take back image of iframe
		/// </summary>
		Image back;

		/// <summary>
		/// take the top left image of ifram
		/// </summary>
		Image topleft;

		/// <summary>
		/// take topright image of ifram
		/// </summary>
		Image topright;

		/// <summary>
		/// take bottomright image of ifram
		/// </summary>
		Image bottomright;

		/// <summary>
		/// take bottomleft image of ifram
		/// </summary>
		Image bottomleft;

		/// <summary>
		/// take right image of iframe
		/// </summary>
		Image right;

		/// <summary>
		/// take left image of ifram
		/// </summary>
		Image left;

		/// <summary>
		/// take the top image of ifram
		/// </summary>
		Image top;

		/// <summary>
		/// take the bottom image of ifram
		/// </summary>
		Image bottom;

		/// <summary>
		/// ??? 
		/// </summary>
		std::string name;

		/// <summary>
		/// position of iframe
		/// </summary>
		int x, y, w, h;
	};
};

#endif