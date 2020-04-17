/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef LIST_H
#define LIST_H



#ifndef MAX_OPTIONS_DEFAULT
#define MAX_OPTIONS_DEFAULT 0
#endif 

#ifndef BORDERWIDTH_DEFAULT
#define BORDERWIDTH_DEFAULT 0
#endif

#include <gl_items.h>

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
		/// This function gets the selected option
		/// </summary>
		/// <returns>Returns the selected option, a std::string.</returns>
		std::string GetSelectedOption() { return selectedOption; }

		/// <summary>
		/// This function creates the text list
		/// </summary>
		/// <param name="_x">X position of the list</param>
		/// <param name="_y">Y position of the list</param>
		/// <param name="_font">Font of the text in the list</param>
		/// <param name="_color">Color of the text in the list</param>
		/// <param name="_backColor">Background color of the list</param>
		/// <param name="_pickingId">Picking ID value for render picking</param>
		/// <param name="luaCmd">LUA command that should be executed. It supports only const string and the default value is empty</param>
		/// <param name="_maxOptions">Max number of options in the list</param>
		/// <param name="_borderWidth">Border thickness of the list</param>
		void Create(int _x, int _y, std::string _font, glm::vec4 _color, glm::vec4 _backColor, int _pickingId, const std::string & luaCmd = "", const unsigned int _maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int _borderWidth =  BORDERWIDTH_DEFAULT);

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

		void Hide(void) { isHidden = true; }

		void Show(void) { isHidden = false; }

		bool IsHidden(void) { return isHidden; }

		bool IsClicked(void);

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

		bool isHidden;
		bool isClicked;

		// variables for scroll
		int minX, maxX, minY, maxY;
		float deltaY;
	};
};

#endif