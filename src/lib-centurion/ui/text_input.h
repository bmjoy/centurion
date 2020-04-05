/*
* ---------------------------
* CENTURION
* [2019] - [2020] Rattlesmake
* All Rights Reserved.
* ---------------------------
*/

#ifndef TEXT_INPUT_H
#define TEXT_INPUT_H

#ifndef TEXT_INPUT_MAX_CHARS_DEFAULT
#define TEXT_INPUT_MAX_CHARS_DEFAULT 100
#endif 

#ifndef MAX_NUMBER_OF_TEXT_INPUTS
#define MAX_NUMBER_OF_TEXT_INPUTS 100
#endif 

#include <primitives.h>

namespace gui {

	/// <summary>
	/// Class of TextInput
	/// </summary>
	class TextInput {
	public:
		TextInput();

		/// <summary>
		/// This function creates the text input
		/// </summary>
		/// <param name="x">x position of text input</param>
		/// <param name="y">y position of the text input</param>
		/// <param name="maxChars">max characters of text input</param>
		/// <param name="text">the placeholder text as wide string</param>
		void Create(const int _id, float x, float y, int maxChars = TEXT_INPUT_MAX_CHARS_DEFAULT, std::wstring placeholderText = L"");

		/// <summary>
		/// This function renders the Input text
		/// </summary>
		void Render();

		/// <summary>
		/// This function enables the text input
		/// </summary>
		void Enable(void) { isActive = true; }

		/// <summary>
		/// This function disables the text input
		/// </summary>
		void Disable(void) { isActive = false; }

		/// <summary>
		/// This method returns the text
		/// </summary>
		/// <returns></returns>
		std::string GetText(void);

#pragma region Static methods

		/// <summary>
		/// This function adds a text input to the static array
		/// </summary>
		/// <param name="id">The TextInput Id</param>
		/// <param name="txtList">The Text input</param>
		static void AddTextInputToArray(int id, TextInput* txtInput);

		/// <summary>
		/// This funtion gets the text input from the array using its ID.
		/// </summary>
		/// <param name="id">The TextInput Id</param>
		/// <returns>Returns a pointer to the required TextInput.</returns>
		static TextInput* GetTextInputById(int id);

#pragma endregion

		~TextInput();
	private:
		gui::Rectangle text_cursor;
		gui::SimpleText static_text;
		bool isActive;
		float xPos, yPos;
		int cursorPosition;
		int max_chars;
		int currentText[100];
		unsigned int id;

		static std::array<TextInput*, MAX_NUMBER_OF_TEXT_INPUTS> TextInputs;
	};
};

#endif