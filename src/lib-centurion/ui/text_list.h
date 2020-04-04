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
		/// This function Add a text list to array
		/// </summary>
		/// <param name="id">Id of the list/array</param>
		/// <param name="txtList">Text list</param>
		static void AddTextListToArray(int id, TextList* txtList);

		/// <summary>
		/// This funtion get the list text from array
		/// </summary>
		/// <param name="id">Id of the array</param>
		/// <returns>return text list</returns>
		static TextList* GetTextListById(int id);

		/// <summary>
		/// This function update text list by id (ma va?)
		/// </summary>
		/// <param name="id">return id(from array)</param>
		/// <param name="_options">???Option to give for update</param>
		/// <param name="prefix">??? prefix of text</param>
		static void UpdateTextListById(int id, std::vector<std::string> *_options, const std::string prefix = "");

		/// <summary>
		/// This function get Id
		/// </summary>
		/// <returns>Id</returns>
		int GetId() { return id; }

		/// <summary>
		/// This function get selected option
		/// </summary>
		/// <returns>Return a strig with selected option</returns>
		std::string GetSelectedOption() { return selectedOption; }

		/// <summary>
		/// This function create the text list
		/// </summary>
		/// <param name="_id">id of array that contain the list</param>
		/// <param name="_x">x position of the list</param>
		/// <param name="_y">y position of the list</param>
		/// <param name="_font">font of text in the list</param>
		/// <param name="_color">color of the text in the list</param>
		/// <param name="_backColor">color of background list</param>
		/// <param name="_pickingId">rendering id</param>
		/// <param name="luaCmd">return lua command to create/show list</param>
		/// <param name="_maxOptions">max option in the list</param>
		/// <param name="_borderWidth">widht border of the list</param>
		void Create(int _id, int _x, int _y, std::string _font, glm::vec4 _color, glm::vec4 _backColor, int _pickingId, const std::string & luaCmd = "", const unsigned int _maxOptions = MAX_OPTIONS_DEFAULT, const unsigned int _borderWidth =  BORDERWIDTH_DEFAULT);

		/// <summary>
		/// This function update the text list
		/// </summary>
		/// <param name="_options">refresh option</param>
		/// <param name="prefix">refresh prefix</param>
		void Update(std::vector<std::string> *_options, const std::string prefix = "");

		/// <summary>
		/// This function render the text list.
		/// </summary>
		/// <param name="picking">Defautl value 0.</param>
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