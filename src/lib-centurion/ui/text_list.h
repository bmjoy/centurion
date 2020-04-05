/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef LIST_H
#define LIST_H

#ifndef MAX_NUMBER_OF_TEXT_LISTS
#define MAX_NUMBER_OF_TEXT_LISTS 100
#endif 

#ifndef MAX_OPTIONS_DEFAULT
#define MAX_OPTIONS_DEFAULT 0
#endif 

#ifndef BORDERWIDTH_DEFAULT
#define BORDERWIDTH_DEFAULT 0
#endif

#include <primitives.h>

#include "simple_text.h"
#include "rectangle.h"

namespace gui {

	/// <summary>
	/// Class of textlist
	/// </summary>
	class TextList {
	public:
		TextList();

		/// <summary>
		/// This function gets the TextList Id
		/// </summary>
		/// <returns>The ID</returns>
		int GetId() { return id; }

		/// <summary>
		/// This function gets the selected option
		/// </summary>
		/// <returns>Returns the selected option, a std::string.</returns>
		std::string GetSelectedOption() { return selectedOption; }

		/// <summary>
		/// This function creates the text list
		/// </summary>
		/// <param name="_id">Array ID that contains the list</param>
		/// <param name="_x">X position of the list</param>
		/// <param name="_y">Y position of the list</param>
		/// <param name="_font">Font of the text in the list</param>
		/// <param name="_color">Color of the text in the list</param>
		/// <param name="_backColor">Background color of the list</param>
		/// <param name="_pickingId">Picking ID value for render picking</param>
		/// <param name="luaCmd">LUA command that should be executed. It supports only const string and the default value is empty</param>
		/// <param name="_maxOptions">Max number of options in the list</param>
		/// <param name="_borderWidth">Border thickness of the list</param>
		void Create(int _id, int _x, int _y, std::string _font, glm::vec4 _color, glm::vec4 _backColor, int _pickingId, const std::string & luaCmd = "", const unsigned int _maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int _borderWidth =  BORDERWIDTH_DEFAULT);

		/// <summary>
		/// This function updates the text list
		/// </summary>
		/// <param name="_options">Refresh options. It supports only referenced string vectors</param>
		/// <param name="prefix">Refresh prefix. It supports only strings and the default value is empty</param>
		void Update(std::vector<std::string> *_options, const std::string prefix = "");

		/// <summary>
		/// This function renders the text list.
		/// </summary>
		/// <param name="picking">Boolean: true = picking, false = normal rendering.</param>
		void Render(bool picking);

	#pragma region Static methods
		/// <summary>
		/// This function adds a text list to the array
		/// </summary>
		/// <param name="id">Id of the text list</param>
		/// <param name="txtList">The TextList element to add</param>
		static void AddTextListToArray(int id, TextList* txtList);

		/// <summary>
		/// This funtion gets the list text from the array
		/// </summary>
		/// <param name="id">Id of the text list</param>
		/// <returns>Returns an element of type TextList</returns>
		static TextList* GetTextListById(int id);

		/// <summary>
		/// This function updates an arbitrary TextList using its ID
		/// </summary>
		/// <param name="id">The id of the text list to update</param>
		/// <param name="_options">List of options</param>
		/// <param name="prefix">The prefix of the text (e.g. "WORD_" if the word has a translation with this prefix)</param>
		static void UpdateTextListById(int id, std::vector<std::string> *_options, const std::string prefix = "");
	#pragma endregion

		~TextList();

	private:
		int GetIdFromClick();
		void ScrollOptions(void);
		bool MouseIsHover(void);
		std::vector<Rectangle> optionsBack;
		std::vector<SimpleText> optionsText;
		std::vector<std::string> options;
		std::string font;
		std::string luaCommand;
		std::string selectedOption;
		glm::vec4 color, backColor;
		gui::Rectangle border;
		glm::vec4 borderColor;
		int borderWidth;
		float optionsHeight;
		int x, y;
		int pickingId;
		int nOptions;
		int maxOptions;
		int firstOption, lastOption;
		int id;

		// variables for scroll
		int minX, maxX, minY, maxY;
		float deltaY;

		static std::array<TextList*, MAX_NUMBER_OF_TEXT_LISTS> TextLists;
	};
};



#endif