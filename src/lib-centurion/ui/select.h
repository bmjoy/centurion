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

namespace gui {

	///Class of the Select; Create a menu with option
	class Select {
	public:

		/// <summary>
		/// ??? This function performs the rendering of the current select.
		/// </summary>
		/// <param name="options"></param>
		Select(bool options = true);

		/// <summary>
		/// This function create the menu.
		/// </summary>
		/// <param name="x">X position of the menu</param>
		/// <param name="y">Y position of the menu</param>
		/// <param name="w">weight of the menu</param>
		/// <param name="h">height of the menu</param>
		/// <param name="options">??? Take options string</param>
		/// <param name="pickingID">Picking ID value for render picking</param>
		void create(float x, float y, float w, float h, std::vector<std::string> options, int pickingID);

		/// <summary>
		/// This function performs the rendiring of the current building.
		/// </summary>
		/// <param name="picking">Dafault value 0.</param>
		/// <param name="color">Color of the ???menu/building. It only support glm::vec4 RGBA</param>
		void render(bool picking, glm::vec4 color);

		/// <summary>
		/// ??? This function open and close the menu.
		/// </summary>
		void open_close();

		/// <summary>
		/// This function close the menu.
		/// </summary>
		void close();

		/// <summary>
		/// ??? This function create a button to show option
		/// </summary>
		/// <param name="i">???</param>
		void select_option(int i);

		/// <summary>
		/// This function create a button to go on with the list
		/// </summary>
		void select_next();

		/// <summary>
		/// This function create a button to go back with the list
		/// </summary>
		void select_previous();

		/// <summary>
		/// This function get the status of the mouse on option nutton. 
		/// </summary>
		/// <returns></returns>
		int get_clicked_option();

		std::string selectedText;
		~Select();
	private:
		Rectangle back;
		Rectangle back_options;
		std::vector<Rectangle> back_options_picking;
		SimpleText text;
		std::vector<SimpleText> optionsText;
		float width, height, x, y;
		std::vector<std::string> form_options;
		glm::vec2 mainTextPos;
		bool boolOptions;
		bool isOpened;
		bool hasText;
		int nOptions;
		int selectedTextID;
	};
};


#endif