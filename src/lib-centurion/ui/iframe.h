/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef IFRAME_H
#define IFRAME_H

#ifndef MAX_NUMBER_OF_TEXT_INPUTS
#define MAX_NUMBER_OF_TEXT_INPUTS 100
#endif 

#ifndef MAX_NUMBER_OF_TEXT_LISTS
#define MAX_NUMBER_OF_TEXT_LISTS 100
#endif 

#ifndef MAX_NUMBER_OF_SIMPLE_TEXT
#define MAX_NUMBER_OF_SIMPLE_TEXT 100
#endif 

#ifndef MAX_NUMBER_OF_TEXT_BOX
#define MAX_NUMBER_OF_TEXT_BOX 100
#endif 

#ifndef MAX_NUMBER_OF_IFRAMES
#define MAX_NUMBER_OF_IFRAMES 100
#endif 

#ifndef MAX_NUMBER_OF_FORM_OPTIONS
#define MAX_NUMBER_OF_FORM_OPTIONS 100
#endif 

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
		/// This static function gets the iframe pointer using its id
		/// </summary>
		/// <param name="_iframeId">The iframe id (a positive integer)</param>
		static Iframe* GetIframeById(const unsigned int _iframeId);

		/// <summary>
		/// This static function clears the iframe pointers array
		/// </summary>
		//static void ClearIframeArray(void);

		/// <summary>
		/// This static function gets the iframe pointer using its id
		/// </summary>
		/// <param name="_iframeId">The iframe id (a positive integer)</param>
		/// <param name="_iframe_ptr">The iframe pointer</param>
		static void AddIframe(const unsigned int _iframeId, Iframe* _iframe_ptr);

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
		void AddButton(const std::wstring &text, int xBtn, int yBtn, const std::string &luaCmd = "");
	
		/// <summary>
		/// This function adds a SimpleText to the iframe texts list.
		/// </summary>
		/// <param name="_textId">The text id</param>
		/// <param name="text">The text to display</param>
		/// <param name="xPos">The xPosition of the text, relative to the iframe x-position</param>
		/// <param name="yPos">The yPosition of the text, relative to the iframe y-position</param>
		void AddText(const unsigned int _textId, const std::wstring &wtext, int xPos, int yPos);

		/// <summary>
		/// This funtion gets the string from the required simple text, identified by its id
		/// </summary>
		/// <param name="_textId">Id of the simple text</param>
		/// <returns>Returns an element of type std::string</returns>
		std::string GetStringBySimpleTextId(const unsigned int _textId);

		/// <summary>
		/// This funtion updates the string of the required simple text, identified by its id
		/// </summary>
		/// <param name="_textId">Id of the simple text</param>
		void UpdateStringBySimpleTextId(const unsigned int _textId, std::string _newText);

		/// <summary>
		/// This function adds a specific text list to the iframe.
		/// </summary>
		/// <param name="textListId">The text list id</param>
		/// <param name="xPos">The text list x-position, relative to the iframe x-position</param>
		/// <param name="yPos">The text list y-position, relative to the iframe y-position</param>
		/// <param name="luaCmd">The Lua command to execute when you click on one option</param>
		/// <param name="maxOptions">The maximum number of options to display</param>
		void AddTextList(const unsigned int textListId, int xPos, int yPos, const std::string & luaCmd = "", const unsigned int maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int borderWidth = BORDERWIDTH_DEFAULT);

		/// <summary>
		/// This funtion gets the list text from the array
		/// </summary>
		/// <param name="id">Id of the text list</param>
		/// <returns>Returns an element of type TextList</returns>
		TextList* GetTextListById(const unsigned int id);

		/// <summary>
		/// This function updates an arbitrary TextList using its ID
		/// </summary>
		/// <param name="id">The id of the text list to update</param>
		/// <param name="_options">List of options</param>
		/// <param name="prefix">The prefix of the text (e.g. "WORD_" if the word has a translation with this prefix)</param>
		void UpdateTextListById(const unsigned int id, std::vector<std::string>* _options, const std::string prefix = "");

		/// <summary>
		/// This function adds a specific text input to the iframe.
		/// </summary>
		/// <param name="textInputId">The text input id</param>
		/// <param name="xPos">The text input x-position, relative to the iframe x-position</param>
		/// <param name="yPos">The text input y-position, relative to the iframe y-position</param>
		/// <param name="width">The text input width</param>
		/// <param name="placeholderText">The placeholder text; default value is an empty string</param>
		void AddTextInput(const unsigned int textInputId, int xPos, int yPos, const int width, std::wstring placeholderText = L"");

		/// <summary>
		/// This funtion gets the text input from the array using its ID.
		/// </summary>
		/// <param name="id">The TextInput Id</param>
		/// <returns>Returns a pointer to the required TextInput.</returns>
		TextInput* GetTextInputById(const unsigned int id);

		/// <summary>
		/// This method updates the placeholder of the required text input, identified with its id
		/// </summary>
		/// <param name="id">The personal idendification value</param>
		/// <param name="newPlaceholder">A std::string indicating the new placeholder text</param>
		void UpdateTextInputPlaceholder(const unsigned int id, std::string newPlaceholder);

		void AddTextBox(const unsigned int textBoxId, const std::wstring& wtext, int xPos, int yPos, const int width, const int height);

		TextBox* GetTextBoxById(const unsigned int id);

		void UpdateTextBoxById(const unsigned int id, const std::wstring * newText);


		void AddFormOptions(const unsigned int formOptionsId, int xPos, int yPos, const int width, const int height);

		FormOptions* GetFormOptionsById(const unsigned int id);

		void UpdateFormOptionsById(const unsigned int id, std::vector<std::string>* _options, const std::string prefix = "");

		/// <summary>
		/// This function creates an instance of Iframe.
		/// </summary>
		/// <param name="_iframeId">The iframe id (a positive integer)</param>
		/// <param name="xPos">X position of the Iframe. It only supports integers.</param>
		/// <param name="yPos">Y position of the Iframe. It only supports integers.</param>
		/// <param name="width">Width of the Iframe. It only supports integers.</param>
		/// <param name="height">Height of the Iframe. It only supports integers.</param>
		/// <param name="iframe_title">The iframe main title.</param>
		void Create(const unsigned int _iframeId, int xPos, int yPos, int width, int height, std::wstring iframe_title = L"");
		
		bool CreateFromXmlElement(tinyxml2::XMLElement * iframe);

		/// <summary>
		/// This function creates an instance of Iframe using a lua command to define size and position
		/// </summary>
		/// <param name="_iframeId">The iframe id (a positive integer)</param>
		/// <param name="LuaCommand">This command must contain the following variables "x, y, w, h" with their values (e.g. "x=1; y=1; w=1; h=1")</param>
		/// /// <param name="iframe_title">The iframe main title.</param>
		void Create(const unsigned int _iframeId, const std::string &LuaCommand, std::wstring iframe_title = L"");

		/// <summary>
		/// This function starts the rendering process of the Iframe.
		/// </summary>
		/// <param name="picking">This boolean parameter enables or disables render picking. Default value is false.</param>
		void Render(bool picking = false);

		/// <summary>
		/// This function returns the iframe id
		/// </summary>
		unsigned int GetId(void) { return iframeId; }

		std::string GetLuaOpeningCMD(void) { return luaOpeningCMD; }

		std::string GetLuaConditionCMD(void) { return luaConditionCMD; }

		std::string GetLuaConditionFUN(void) { return luaConditionFUN; }

		bool IsOpened(void) { return isOpened; }

		void Open(void) { isOpened = true; opening = true; }

		void Close(void) { isOpened = false; }

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

		void RenderTextBoxes(bool picking = false);

		void RenderFormOptions(bool picking);

		void RenderBackgroundImages(bool picking);


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
		std::array<SimpleText*, MAX_NUMBER_OF_SIMPLE_TEXT> listOfTexts;
		std::array<TextList*, MAX_NUMBER_OF_TEXT_LISTS> listOfTextLists;
		std::array<TextInput*, MAX_NUMBER_OF_TEXT_INPUTS> listOfTextInputs;
		std::array<TextBox*, MAX_NUMBER_OF_TEXT_BOX> listOfTextBoxes;
		std::array<FormOptions*, MAX_NUMBER_OF_FORM_OPTIONS> listOfFormOptions;
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
		unsigned int iframeId;

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

		// lua 
		std::string luaOpeningCMD;
		std::string luaConditionCMD;
		std::string luaConditionFUN;

		bool isOpened;
		bool opening;

		static std::array<Iframe*, MAX_NUMBER_OF_IFRAMES> iframes;
	};
};

#endif