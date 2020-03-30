#ifndef LIST_H
#define LIST_H

#define MAX_NUMBER_OF_TEXT_LISTS 100

#include <primitives.h>

#include "simple_text.h"
#include "rectangle.h"

namespace gui {
	class TextList {
	public:
		TextList();
		static void AddTextListToArray(int id, TextList* txtList);
		static TextList* GetTextListById(int id);
		static void UpdateTextListById(int id, std::vector<std::string> *_options);
		int GetId() { return id; }
		std::string GetSelectedOption() { return selectedOption; }
		void Create(int _id, int _x, int _y, std::string _font, glm::vec4 _color, glm::vec4 _backColor, int _pickingId);
		void Update(std::vector<std::string> *_options);
		void Render(bool picking);
		~TextList();

	private:
		int GetIdFromClick();
		std::vector<Rectangle> optionsBack;
		std::vector<SimpleText> optionsText;
		std::vector<std::string> options;
		std::string font;
		std::string selectedOption;
		glm::vec4 color, backColor;
		float optionsHeight;
		int x, y;
		int pickingId;
		int n_options;
		int id;

		static std::array<TextList*, MAX_NUMBER_OF_TEXT_LISTS> TextLists;
	};
};



#endif