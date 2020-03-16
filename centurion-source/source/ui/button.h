#ifndef BUTTON_H
#define BUTTON_H

#include <primitives.h>

namespace gui {

	class SimpleText;
	class Image;

	class Button {
	public:
		Button();
		string GetLuaCommand() { return luaCmd; }
		void create(string imageName, string Text, int x, int y, int pickingID, vec4 textColor, string luaCMD = "");
		void render(bool picking, int leftClickId = 0);
		~Button();
	private:
		SimpleText *buttonText;
		Image *buttonImage;
		string luaCmd;
		int pickingId;
	};
};

#endif