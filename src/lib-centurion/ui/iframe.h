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
		/// <param name="text">The button text</param>
		/// <param name="xBtn">The button x-position, relative to the iframe x-position</param>
		/// <param name="yBtn">The button y-position, relative to the iframe y-position</param>
		/// <param name="luaCmd">The button Lua command. Default = empty string</param>
		void AddButton(const std::string &text, const int xBtn, const int yBtn, const std::string &luaCmd = "");

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
		/// <param name="textListId">The text list id</param>
		/// <param name="xPos">The text list x-position, relative to the iframe x-position</param>
		/// <param name="yPos">The text list y-position, relative to the iframe y-position</param>
		/// <param name="luaCmd">The Lua command to execute when you click on one option</param>
		void AddTextList(const int textListId, const int xPos, const int yPos, const std::string & luaCmd = "");

		/// <summary>
		/// This function creates an instance of Iframe.
		/// </summary>
		/// <param name="xPos">X postition of the Iframe. It only supports integers.</param>
		/// <param name="yPos">Y postition of the Iframe. It only supports integers.</param>
		/// <param name="width">Width of the Iframe. It only supports integers.</param>
		/// <param name="height">Height of the Iframe. It only supports integers.</param>
		void Create(int xPos, int yPos, int width, int height);
		
		/// <summary>
		/// This function creates an instance of Iframe using a lua command to define size and position
		/// </summary>
		/// <param name="LuaCommand">This command must contain the following variables "x, y, w, h" with their values (e.g. "x=1; y=1; w=1; h=1")</param>
		void Create(const std::string &LuaCommand);

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

		/// <summary>
		/// This method reads the data contained in the XML file of the selected iframe
		/// </summary>
		void ReadXml();

		/// <summary>
		/// This method creates the background image
		/// </summary>
		void CreateBackgroundImage();

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
		std::string back_name;
		std::string topleft_name;
		std::string topright_name;
		std::string bottomright_name;
		std::string bottomleft_name;
		std::string right_name;
		std::string left_name;
		std::string top_name;
		std::string bottom_name;
		std::string name;
		int x, y, w, h;
		bool backgroundIsCreated;

		// button data
		std::string button_img_name, button_font;
		glm::vec4 button_txt_color;

		// textListData
		std::string text_list_font; 
		glm::vec4 text_list_background, text_list_color;
	};
};

#endif