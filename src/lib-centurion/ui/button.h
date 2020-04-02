#ifndef BUTTON_H
#define BUTTON_H

#include <primitives.h>

namespace gui {

	class SimpleText;
	class Image;

	/// <summary>
	/// Class of Buttun
	/// </summary>
	class Button {
	public:

		/// <summary>
		/// Button Costructor
		/// </summary>
		Button();

		/// <summary>
		/// This function get Lua Command of Button
		/// </summary>
		/// <returns>return luaCmd</returns>
		std::string GetLuaCommand() { return luaCmd; }

		/// <summary>
		/// This function create Button
		/// </summary>
		/// <param name="imageName">Name of Button image</param>
		/// <param name="Text">Text (wstring) in Button </param>
		/// <param name="x">X Position of Button</param>
		/// <param name="y">y Position of Button</param>
		/// <param name="pickingID">The picking ID to use</param>
		/// <param name="textColor">Text Color in Button</param>
		/// <param name="luaCMD">Command in lua to creat Button</param>
		void create(std::string imageName, std::wstring Text, int x, int y, int pickingID, glm::vec4 textColor, std::string luaCMD = "");

		/// <summary>
		/// This function render the button 
		/// </summary>
		/// <param name="picking">Bool parameter that determines picking state during rendering</param>
		/// <param name="leftClickId">Default value 0</param>
		void render(bool picking, int leftClickId = 0);

		/// <summary>
		/// Destructor of Button
		/// </summary>
		~Button();
	private:

		/// <summary>
		/// Text of the button
		/// </summary>
		SimpleText *buttonText;
		
		/// <summary>
		/// Image of the button
		/// </summary>
		Image *buttonImage;
		
		/// <summary>
		/// Return a lua command
		/// </summary>
		std::string luaCmd;

		/// <summary>
		/// ???
		/// </summary>
		int pickingId;
	};
};

#endif