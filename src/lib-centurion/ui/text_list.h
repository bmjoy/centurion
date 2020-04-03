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
	class TextList {
	public:
		TextList();
		static void AddTextListToArray(int id, TextList* txtList);
		static TextList* GetTextListById(int id);
		static void UpdateTextListById(int id, std::vector<std::string> *_options, const std::string prefix = "");
		int GetId() { return id; }
		std::string GetSelectedOption() { return selectedOption; }
		void Create(int _id, int _x, int _y, std::string _font, glm::vec4 _color, glm::vec4 _backColor, int _pickingId, const std::string & luaCmd = "", const unsigned int _maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int _borderWidth =  BORDERWIDTH_DEFAULT);
		void Update(std::vector<std::string> *_options, const std::string prefix = "");
		void Render(bool picking);
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