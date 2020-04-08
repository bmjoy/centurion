/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef IFRAME_H
#define IFRAME_H

#include <gl_items.h>

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
		/// This function clears the entire texts list. 
		/// </summary>
		void Clear();

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
		void AddButton(const std::wstring &text, const int xBtn, const int yBtn, const std::string &luaCmd = "");
	
		/// <summary>
		/// This function adds a SimpleText to the iframe texts list.
		/// </summary>
		/// <param name="text">The text to display</param>
		/// <param name="xPos">The xPosition of the text, relative to the iframe x-position</param>
		/// <param name="yPos">The yPosition of the text, relative to the iframe y-position</param>
		void AddText(const std::wstring &wtext, const int xPos, const int yPos);

		/// <summary>
		/// This function adds a specific text list to the iframe.
		/// </summary>
		/// <param name="textListId">The text list id</param>
		/// <param name="xPos">The text list x-position, relative to the iframe x-position</param>
		/// <param name="yPos">The text list y-position, relative to the iframe y-position</param>
		/// <param name="luaCmd">The Lua command to execute when you click on one option</param>
		/// <param name="maxOptions">The maximum number of options to display</param>
		void AddTextList(const int textListId, const int xPos, const int yPos, const std::string & luaCmd = "", const unsigned int maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int borderWidth = BORDERWIDTH_DEFAULT);

		/// <summary>
		/// This function adds a specific text input to the iframe.
		/// </summary>
		/// <param name="textInputId">The text input id</param>
		/// <param name="xPos">The text input x-position, relative to the iframe x-position</param>
		/// <param name="yPos">The text input y-position, relative to the iframe y-position</param>
		/// <param name="width">The text input width</param>
		/// <param name="placeholderText">The placeholder text; default value is an empty string</param>
		void AddTextInput(const int textInputId, const int xPos, const int yPos, const int width, std::wstring placeholderText = L"");

		/// <summary>
		/// This function creates an instance of Iframe.
		/// </summary>
		/// <param name="xPos">X position of the Iframe. It only supports integers.</param>
		/// <param name="yPos">Y position of the Iframe. It only supports integers.</param>
		/// <param name="width">Width of the Iframe. It only supports integers.</param>
		/// <param name="height">Height of the Iframe. It only supports integers.</param>
		/// <param name="iframe_title">The iframe main title.</param>
		void Create(int xPos, int yPos, int width, int height, std::wstring iframe_title = L"");
		
		/// <summary>
		/// This function creates an instance of Iframe using a lua command to define size and position
		/// </summary>
		/// <param name="LuaCommand">This command must contain the following variables "x, y, w, h" with their values (e.g. "x=1; y=1; w=1; h=1")</param>
		/// /// <param name="iframe_title">The iframe main title.</param>
		void Create(const std::string &LuaCommand, std::wstring iframe_title = L"");

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
		void RenderImages(bool picking = false);
		
		/// <summary>
		/// This function starts the rendering process of buttons within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderButtons(bool picking = false);

		/// <summary>
		/// This function starts the rendering process of texts within the Iframe text list.
		/// </summary>
		void RenderTexts(bool picking = false);

		/// <summary>
		/// This function starts the rendering process of text lists within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderTextLists(bool picking = false);

		/// <summary>
		/// This function starts the rendering process of text inputs within the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void RenderTextInputs(bool picking = false);

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
		std::vector<TextInput*> listOfTextInputs;
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

		// iframe title
		gui::SimpleText iframeTitle;

		// button data
		std::string button_img_name, button_font;
		glm::vec4 button_txt_color;

		// textList Data
		std::string text_list_font; 
		glm::vec4 text_list_background, text_list_color;
		
		// textInput Data
		std::string text_input_font, text_input_fontweight;
		glm::vec4 text_input_background, text_input_border, text_input_color;
		bool text_input_has_background;
	};
};

#endif