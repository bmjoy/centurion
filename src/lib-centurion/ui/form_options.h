/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef SELECT_H
#define SELECT_H

#include <gl_items.h>

#include "rectangle.h"
#include "simple_text.h"
#include "text_list.h"

namespace gui {

	///Class of the Select; Create a menu with option
	class FormOptions {
	public:

		/// <summary>
		/// ??? This function performs the rendering of the current select.
		/// </summary>
		FormOptions();

		/// <summary>
		/// This function create the menu.
		/// </summary>
		void Create(const unsigned int pickingId, const int xPos, const int yPos, const int width, const int height, const std::string font, const glm::vec4 _textColor, const glm::vec4 _backColor);

		/// <summary>
		/// This function performs the rendiring of the current building.
		/// </summary>
		/// <param name="picking">Dafault value 0.</param>
		/// <param name="color">Color of the ???menu/building. It only support glm::vec4 RGBA</param>
		void Render(bool picking);

		void UpdateOptions(std::vector<std::string>* optionsList, std::string prefix = "");

		std::wstring GetSelectedOption(void);
		~FormOptions();
	private:
		Rectangle selectedOptionBack;
		SimpleText selectedOptionText;
		std::wstring selectedText;
		std::string fontName;
		glm::vec4 textColor;
		glm::vec4 backColor;

		TextList options;
		std::vector<std::string> optionsList;
		std::string prefix;
		float w, h, x, y;
	};
};


#endif